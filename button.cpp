#include "button.h"
#include <arduino.h>

BUTTON::BUTTON(){
  this->history = 0;
  // look for 00xxxx1111111111
  this->pattern_pressed_mask = 0xC3FF;
  this->pattern_pressed = 0x03FF;
  // look for 1111xxxx00000000
  this->pattern_released_mask = 0xF0FF;
  this->pattern_released = 0xF000;

  this->pressed = false;
  this->held = false;
  this->released = false;

  // default mode set
  this->mode = BUTTON_MODE_ACTIVE_LOW;

  return;
}

BUTTON::~BUTTON(){
  return;
}

void BUTTON::setMode(uint16_t flags){
  this->mode = flags;
}

void BUTTON::set(int p, uint16_t flags){
  this->setMode(flags);
  this->setPin(p);
}

void BUTTON::setPin(int p){
  this->pin = p;
}

void BUTTON::init(){
  if ( this->mode & BUTTON_MODE_INTERNAL_PULLUP )
    pinMode(this->pin, INPUT_PULLUP);
  else
    pinMode(this->pin, INPUT);
  return;
}

void BUTTON::update(){
  // Read the input value from the button
  int val = digitalRead(this->pin);
  if ( this->mode & BUTTON_MODE_ACTIVE_LOW )
    val = !val;

  if ( this->mode & BUTTON_MODE_NO_DEBOUNCE ){
    this->pressed = !this->held && val;
    this->released = this->held && !val;
    this->held = val;
  }
  else {
    this->history = ( (this->history)<<1 ) | (val?0x1:0x0);
    if ( (this->history & this->pattern_pressed_mask) == this->pattern_pressed ){
      Serial.println("button press");
      this->pressed = true;
      this->held = true;
      this->released = false;
      this->history = 0xFFFF;
    }
    else if ( (this->history & this->pattern_released_mask) == this->pattern_released ){
      this->released = true;
      this->held = false;
      this->pressed = false;
      this->history = 0x0000;
    }
    else {
      this->pressed = false;
      this->released = false;
    }
  }
}

/*
  ACCESSORS
*/
bool BUTTON::isHeld(){
  return this->held;
}

bool BUTTON::isPressed(){
  return this->pressed;
}

bool BUTTON::isReleased(){
  return this->released;
}

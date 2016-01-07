#ifndef __BUTTON_H__
#define __BUTON_H__

#include <arduino.h>

// Define button mode flags
#define BUTTON_MODE_ACTIVE_LOW 0x1
#define BUTTON_MODE_INTERNAL_PULLUP 0x2
#define BUTTON_MODE_NO_DEBOUNCE 0x4

class BUTTON {

  public:
    BUTTON();
    ~BUTTON();

    void set(int p, uint16_t flags);
    void setPin(int p);
    void setMode(uint16_t flags);
    void init();
    void update();

    bool isHeld();
    bool isPressed();
    bool isReleased();

  private:
    uint16_t history;
    uint16_t pattern_pressed_mask;
    uint16_t pattern_pressed;
    uint16_t pattern_released_mask;
    uint16_t pattern_released;
    bool pressed;
    bool held;
    bool released;

    int pin;
    uint16_t mode;
};

#endif

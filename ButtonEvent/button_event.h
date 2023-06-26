//
// Created by Monster on 2023/6/26.
//

#ifndef HC32L13X_BUTTON_EVENT_H
#define HC32L13X_BUTTON_EVENT_H

#include "stdint.h"
#include "stdbool.h"

typedef void *ButtonHandle_t;
typedef ButtonHandle_t ButtonID;

/**
 * Enable dynamic memory create button
 */
#define BUTTON_EVENT_DYNAMIC_MEMORY_EN 0

/**
 * Button object max number
 */
#if !BUTTON_EVENT_DYNAMIC_MEMORY_EN
#define BUTTON_MAX_NUM  5
#endif

typedef enum {
    EVENT_NONE,
    EVENT_PRESSED,
    EVENT_PRESSING,
    EVENT_LONG_PRESSED,
    EVENT_LONG_PRESSED_REPEAT,
    EVENT_LONG_PRESSED_RELEASED,
    EVENT_RELEASED,
    EVENT_CHANGED,
    EVENT_CLICKED,
    EVENT_SHORT_CLICKED,
    EVENT_DOUBLE_CLICKED,
    EVENT_LAST
} ButtonEvent_t;

/* Button state type */
typedef enum {
    STATE_NO_PRESS,
    STATE_PRESS,
    STATE_LONG_PRESS
} ButtonState_t;

/**
 * Button object description
 */
typedef struct ButtonDef_s {
    char *name;             //!< Button name
    ButtonState_t nowState;       //!< Current state
    struct {
        uint16_t longPressTimeCfg;
        uint16_t longPressRepeatTimeCfg;
        uint16_t doubleClickTimeCfg;
        uint32_t lastLongPressTime;
        uint32_t lastPressTime;
        uint32_t lastClickTime;
        uint16_t clickCnt;
        bool isLongPressed;
    } priv;

    void (*callback)(ButtonID btn, int event);     //!< Button callback function
    bool (*getPress)();     //!< Get button press status function pointer
} ButtonDef_t;


ButtonID ButtonCreate(const char *name, bool (*getPress)(), void (*callback)(ButtonID, int),
                      uint16_t longPressTime, uint16_t longPressTimeRepeat, uint16_t doubleClickTime);

void ButtonRemove(ButtonID id);

void ButtonMonitor();

#endif //HC32L13X_BUTTON_EVENT_H

//
// Created by Monster on 2023/6/26.
//

#ifndef _BUTTON_EVENT_H
#define _BUTTON_EVENT_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus


#include "stdint.h"
#include "stdbool.h"

typedef void *ButtonHandle_t;
typedef ButtonHandle_t ButtonID;

/**
 * Enable dynamic memory create button
 */
#define BUTTON_EVENT_DYNAMIC_MEMORY_EN 1

/**
 * Button object max number
 */
#if !BUTTON_EVENT_DYNAMIC_MEMORY_EN
#define BUTTON_MAX_NUM  5
#endif

typedef enum {
#define EVENT_DEF(evt) evt

#include "EventType.inc"

#undef EVENT_DEF
    EVENT_LAST
} ButtonEvent_t;

/* Button state type */
typedef enum {
    STATE_NO_PRESS,
    STATE_PRESS,
    STATE_LONG_PRESS
} ButtonState_t;

/**
 * Button trigger record
 */
typedef struct {
    bool pressed;
    bool clicked;
    bool longPressed;
    uint16_t clickCnt;
} ButtonRecord_t;

/**
 * Button object description
 */
typedef struct ButtonDef_s {
    char *name;             //!< Button name
    ButtonState_t nowState; //!< Current state
    ButtonRecord_t record;  //!< Button record
    struct {
        uint16_t longPressTimeCfg;
        uint16_t longPressRepeatTimeCfg;
        uint16_t doubleClickTimeCfg;
        uint32_t lastLongPressTime;
        uint32_t lastPressTime;
        uint32_t lastClickTime;
        bool isLongPressed;
    } priv;

    void (*callback)(ButtonID id, int event);     //!< Button callback function
    bool (*getPress)(char *name);     //!< Get button press status function pointer
} ButtonDef_t;


ButtonID ButtonCreate(const char *name, bool (*getPress)(char *name), void (*callback)(ButtonID, int),
                      uint16_t longPressTime, uint16_t longPressTimeRepeat, uint16_t doubleClickTime);

void ButtonRemove(ButtonID id);

bool ButtonGetRecord(const char *name, ButtonRecord_t* record);

void ButtonMonitor();

#ifdef __cplusplus
} // extern "C" {
#endif // __cplusplus
#endif // _BUTTON_EVENT_H

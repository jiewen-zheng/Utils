//
// Created by Monster on 2023/6/26.
//

#ifndef BUTTONEVENT_H
#define BUTTONEVENT_H

#include <stdint.h>

class ButtonEvent {
private:
    typedef void(*FuncCallback_t)(ButtonEvent *btn, int event);

public:
    /* Button event */
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
    } Event_t;

public:
    bool IsPressed;
    bool IsClicked;
    bool IsLongPressed;

public:
    ButtonEvent(
            uint16_t longPressTime = 500,
            uint16_t longPressTimeRepeat = 200,
            uint16_t doubleClickTime = 200
    );

    void EventAttach(FuncCallback_t function);

    void EventMonitor(bool isPress);

    inline uint16_t GetClickCnt() {
        uint16_t cnt = priv.clickCnt;
        priv.clickCnt = 0;
        return cnt;
    }

    inline bool GetClicked() {
        bool n = IsClicked;
        IsClicked = false;
        return n;
    }

    inline bool GetPressed() {
        bool n = IsPressed;
        IsPressed = false;
        return n;
    }

    inline bool GetLongPressed() {
        bool n = IsLongPressed;
        IsLongPressed = false;
        return n;
    }

    inline uint8_t GetState() {
        return priv.nowState;
    }

private:
    /* Button state type */
    typedef enum {
        STATE_NO_PRESS,
        STATE_PRESS,
        STATE_LONG_PRESS
    } State_t;

private:
    struct {
        State_t nowState;
        uint16_t longPressTimeCfg;
        uint16_t longPressRepeatTimeCfg;
        uint16_t doubleClickTimeCfg;
        uint32_t lastLongPressTime;
        uint32_t lastPressTime;
        uint32_t lastClickTime;
        uint16_t clickCnt;
        bool isLongPressed;
        FuncCallback_t eventCallback;
    } priv;

private:
    uint32_t GetTickElaps(uint32_t prevTick);
};


#endif //BUTTONEVENT_H

//
// Created by Monster on 2023/6/26.
//

#include "ButtonEvent.h"
#include "string.h"

#include "delay.h"

#define GET_TICK() millis()

#ifndef GET_TICK
#error "Please define the GET_TICK() function to get the system time."
#endif

#ifndef UINT32_MAX
#define UINT32_MAX 0xffffffffU  /* 4294967295U */
#endif

/**
 * 按键事件构造函数
 * @param longPressTime 按键长按触发超时设置
 * @param longPressTimeRepeat 长按重复触发时间
 * @param doubleClickTime 双击触发时间
 */
ButtonEvent::ButtonEvent(uint16_t longPressTime,
                         uint16_t longPressTimeRepeat,
                         uint16_t doubleClickTime) {

    memset(&priv, 0, sizeof(priv));

    priv.longPressTimeCfg = longPressTime;
    priv.longPressRepeatTimeCfg = longPressTimeRepeat;
    priv.doubleClickTimeCfg = doubleClickTime;

    priv.lastLongPressTime = priv.lastClickTime = priv.lastPressTime = 0;
    priv.isLongPressed = false;
    priv.nowState = STATE_NO_PRESS;

    priv.eventCallback = NULL;
}

/**
 * 按键事件绑定
 * @param function 回调函数指针
 */
void ButtonEvent::EventAttach(ButtonEvent::FuncCallback_t function) {
    priv.eventCallback = function;
}

/**
 * 监控事件，建议扫描周期10ms
 * @param isPress 当前按键状态
 */
void ButtonEvent::EventMonitor(bool isPress) {
    if (priv.eventCallback == NULL) {
        return;
    }

    if (isPress && priv.nowState == STATE_NO_PRESS) {
        priv.nowState = STATE_PRESS;

        IsPressed = true;
        priv.lastPressTime = GET_TICK();

        priv.eventCallback(this, EVENT_PRESSED);
        priv.eventCallback(this, EVENT_CHANGED);
    }

    if (priv.nowState == STATE_NO_PRESS) {
        return;
    }

    if (isPress) {
        priv.eventCallback(this, EVENT_PRESSING);
    }

    if (isPress && GetTickElaps(priv.lastPressTime) >= priv.longPressTimeCfg) {
        priv.nowState = STATE_LONG_PRESS;

        if (!priv.isLongPressed) {
            priv.eventCallback(this, EVENT_LONG_PRESSED);
            priv.lastLongPressTime = GET_TICK();
            IsLongPressed = priv.isLongPressed = true;
        } else if (GetTickElaps(priv.lastLongPressTime) >= priv.longPressRepeatTimeCfg) {
            priv.lastLongPressTime = GET_TICK();
            priv.eventCallback(this, EVENT_LONG_PRESSED_REPEAT);
        }
    } else if (!isPress) {
        priv.nowState = STATE_NO_PRESS;

        if (GetTickElaps(priv.lastClickTime) < priv.doubleClickTimeCfg) {
            priv.clickCnt++;
            priv.eventCallback(this, EVENT_DOUBLE_CLICKED);
        }

        if (priv.isLongPressed) {
            priv.eventCallback(this, EVENT_LONG_PRESSED_RELEASED);
        }

        priv.isLongPressed = false;
        IsClicked = true;
        priv.lastClickTime = GET_TICK();

        if (GetTickElaps(priv.lastPressTime) < priv.longPressTimeCfg) {
            priv.eventCallback(this, EVENT_SHORT_CLICKED);
        }

        priv.eventCallback(this, EVENT_CLICKED);
        priv.eventCallback(this, EVENT_RELEASED);
        priv.eventCallback(this, EVENT_CHANGED);
    }

}

/**
 * 获取与上次时间的时间差
 * @param prevTick 上次的时间戳
 * @retval 时间差
 */
uint32_t ButtonEvent::GetTickElaps(uint32_t prevTick) {
    uint32_t actTime = GET_TICK();

    if (actTime >= prevTick) {
        prevTick = actTime - prevTick;
    } else {
        prevTick = UINT32_MAX - prevTick + 1;
        prevTick += actTime;
    }

    return prevTick;
}

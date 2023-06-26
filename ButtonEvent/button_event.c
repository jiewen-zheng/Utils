//
// Created by Monster on 2023/6/26.
//

#include "button_event.h"
#include "simple_list.h"

#include "string.h"

#include "delay.h"

#define GET_TICK() millis()

#ifndef GET_TICK
#error "Please define the GET_TICK() function to get the system time."
#endif

#ifndef UINT32_MAX
#define UINT32_MAX 0xffffffffU  /* 4294967295U */
#endif

#if !BUTTON_EVENT_DYNAMIC_MEMORY_EN
ButtonDef_t Button[BUTTON_MAX_NUM];
#endif

// 创建链表
simpleListDef(btnList);
List_t *btnList = simpleList(btnList);
ListMethod_t *btnMethod = simpleListMethod(btnList);
static bool list_init = false;


int btnIsExists(ButtonID id) {
    int size = btnMethod->size(btnList);
    for (int i = 0; i < size; i++) {
        ButtonID *pItem = (ButtonID *) btnMethod->get(btnList, i);
        if (pItem == id) {
            return i;
        }
    }

    return -1;
}

ButtonID checkName(char *name) {
    int size = btnMethod->size(btnList);
    for (int i = 0; i < size; i++) {
        ButtonDef_t *pItem = (ButtonDef_t *) btnMethod->get(btnList, i);
        if (strcmp(pItem->name, name) == 0) {
            return pItem;
        }
    }

    return NULL;
}


/**
 * 创建按键对象
 * @param name 按键名称
 * @param callback 回调函数指针
 * @param data 用户参数指针
 * @param longPressTime 按键长按触发超时设置
 * @param longPressTimeRepeat 长按重复触发时间
 * @param doubleClickTime 双击触发时间
 * @retval 按键对象句柄
 */
ButtonID ButtonCreate(const char *name, bool (*getPress)(), void (*callback)(ButtonID, int),
                      uint16_t longPressTime, uint16_t longPressTimeRepeat, uint16_t doubleClickTime) {

    // 链表初始化
    if (!list_init) {
        list_init = true;
        simpleListInit(btnList);
    }

    if (name == NULL || getPress == NULL || callback == NULL) {
        return NULL;
    }

    if (checkName((char *) name)) {
        return NULL;
    }

    ButtonDef_t *button = NULL;
#if BUTTON_EVENT_DYNAMIC_MEMORY_EN
    button = (ButtonDef_t * ) malloc(sizeof(ButtonDef_t));

    if(button == NULL){
        return NULL;
    }
#else
    for (int i = 0; i < BUTTON_MAX_NUM; i++) {
        if (Button[i].name == NULL) {
            button = &Button[i];
            break;
        }
    }

    if (button == NULL) {
        return NULL;
    }
#endif
    memset(button, 0, sizeof(ButtonDef_t));

    button->priv.longPressTimeCfg = longPressTime;
    button->priv.longPressRepeatTimeCfg = longPressTimeRepeat;
    button->priv.doubleClickTimeCfg = doubleClickTime;

    button->priv.lastLongPressTime = button->priv.lastClickTime = button->priv.lastPressTime = 0;
    button->priv.isLongPressed = false;

    button->name = (char *) name;
    button->nowState = STATE_NO_PRESS;
    button->getPress = getPress;
    button->callback = callback;

    if (!btnMethod->add(btnList, button)) {
#if BUTTON_EVENT_DYNAMIC_MEMORY_EN
        free(button);
#endif
        return NULL;
    }

    return button;
}

/**
 * 删除按键对象
 * @param id 按键句柄
 */
void ButtonRemove(ButtonID id) {
    int index = btnIsExists(id);
    if (index == -1) {
        // this button not exists
        return;
    }

    ButtonDef_t *btn = (ButtonDef_t *) btnMethod->remove(btnList, index);

#if BUTTON_EVENT_DYNAMIC_MEMORY_EN
    // free the button memory
    free(btn);
#else
    // clear array
    memset(btn, 0, sizeof(ButtonDef_t));
#endif
}

/**
 * 获取与上次时间的时间差
 * @param prevTick 上次的时间戳
 * @retval 时间差
 */
static uint32_t GetTickElaps(uint32_t prevTick) {
    uint32_t actTime = GET_TICK();

    if (actTime >= prevTick) {
        prevTick = actTime - prevTick;
    } else {
        prevTick = UINT32_MAX - prevTick + 1;
        prevTick += actTime;
    }

    return prevTick;
}

/**
 * 监控事件，建议扫描周期10ms
 * @param isPress 当前按键状态
 */
void ButtonMonitor() {
    int size = btnMethod->size(btnList);
    if (size <= 0) {
        return;
    }

    ButtonDef_t *btn = NULL;
    for (int i = 0; i < size; i++) {
        btn = (ButtonDef_t *) btnMethod->get(btnList, i);
        if (!btn) {
            continue;
        }

        bool isPress = btn->getPress();

        if (isPress && btn->nowState == STATE_NO_PRESS) {
            btn->nowState = STATE_PRESS;

            btn->priv.lastPressTime = GET_TICK();

            btn->callback(btn, EVENT_PRESSED);
            btn->callback(btn, EVENT_CHANGED);
        }

        if (btn->nowState == STATE_NO_PRESS) {
            continue;
        }

        if (isPress) {
            btn->callback(btn, EVENT_PRESSING);
        }

        if (isPress && GetTickElaps(btn->priv.lastPressTime) >= btn->priv.longPressTimeCfg) {
            btn->nowState = STATE_LONG_PRESS;

            if (!btn->priv.isLongPressed) {
                btn->callback(btn, EVENT_LONG_PRESSED);
                btn->priv.lastLongPressTime = GET_TICK();
                btn->priv.isLongPressed = true;
            } else if (GetTickElaps(btn->priv.lastLongPressTime) >= btn->priv.longPressRepeatTimeCfg) {
                btn->priv.lastLongPressTime = GET_TICK();
                btn->callback(btn, EVENT_LONG_PRESSED_REPEAT);
            }
        } else if (!isPress) {
            btn->nowState = STATE_NO_PRESS;

            if (GetTickElaps(btn->priv.lastClickTime) < btn->priv.doubleClickTimeCfg) {
                btn->priv.clickCnt++;
                btn->callback(btn, EVENT_DOUBLE_CLICKED);
            }

            if (btn->priv.isLongPressed) {
                btn->callback(btn, EVENT_LONG_PRESSED_RELEASED);
            }

            btn->priv.isLongPressed = false;

            btn->priv.lastClickTime = GET_TICK();

            if (GetTickElaps(btn->priv.lastPressTime) < btn->priv.longPressTimeCfg) {
                btn->callback(btn, EVENT_SHORT_CLICKED);
            }

            btn->callback(btn, EVENT_CLICKED);
            btn->callback(btn, EVENT_RELEASED);
            btn->callback(btn, EVENT_CHANGED);
        }

    }

}


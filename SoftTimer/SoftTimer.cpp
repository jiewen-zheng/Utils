//
// Created by moon on 2023/6/15.
//

#include "SoftTimer.h"
#include <string.h>


#if !SOFT_TIMER_DYNAMIC_EN
SoftTimerDef_t TimerInstance[SOFT_TIMER_MAX_NUM];
#endif


/**
 * Set get timestamp function
 * @param get_Time
 */
void SoftTimer::setTime(SoftTimerGetTime_t get_Time){
    stGetTime = get_Time;
}

/**
 * Dynamic create timer
 * @param name - timer name
 * @param period - timer period
 * @param callback - timer timeout callback function pointer
 * @param data - User data
 * @param mode - Timer run mode
 * @return timer object or false
 */
softTimerID SoftTimer::create(const char *name, uint32_t period, void (*callback)(void *), void *data, stMode_t mode) {

    if (isExists(name) || callback == nullptr) {
        // timer is exists.
        return nullptr;
    }

    SoftTimerDef_t *new_timer = nullptr;
#if SOFT_TIMER_DYNAMIC_EN
    new_timer = new SoftTimerDef_t;

    if (new_timer == nullptr) {
        return nullptr;
    }
#else
    for(int i=0;i< SOFT_TIMER_MAX_NUM;i++){
        if(TimerInstance[i].name == nullptr){
            new_timer = &TimerInstance[i];
            break;
        }
    }

    if(!new_timer){
        return nullptr;
    }
#endif

    new_timer->name = (char *) name;
    new_timer->period = (period > 0) ? period : 1;
    new_timer->mode = mode;
    new_timer->isStarted = false;
    new_timer->callback = callback;
    new_timer->data = data;

    if (!list.add(new_timer)) {
        return nullptr;
    }

    return new_timer;
}

/**
 * Remove timer.
 * - Only the dynamic application timer
 * @param name - Timer name
 */
void SoftTimer::remove(const char *name) {
    softTimerID id = isExists(name);

    if (!id) {
        return;
    }

    remove(id);
}


void SoftTimer::remove(softTimerID id) {
    int index = isExists(id);
    if (index == -1) {
        return;
    }

    // remove list none
    auto *remove_timer = (SoftTimerDef_t *) list.remove(index);

#if SOFT_TIMER_DYNAMIC_EN
    // free the timer memory
    delete remove_timer;
#else
    memset(remove_timer, 0, sizeof(SoftTimerDef_t));
#endif

}


softTimerID SoftTimer::isExists(const char *name) {
    int size = list.size();
    for (int i = 0; i < size; i++) {
        auto *pItem = list.get(i);
        if (strcmp(pItem->name, name) == 0) {
            return pItem;
        }
    }

    return nullptr;
}

int SoftTimer::isExists(softTimerID id) {
    int size = list.size();
    for (int i = 0; i < size; i++) {
        auto *pItem = list.get(i);
        if (pItem == id) {
            return i;
        }
    }

    return -1;
}


void SoftTimer::start(const char *name) {
    softTimerID id = isExists(name);

    if (!id) {
        return;
    }

    start(id);
}

void SoftTimer::start(softTimerID id) {
    if (!id) {
        return;
    }

    auto *timer = (SoftTimerDef_t *) id;

    timer->time = SOFT_TIMER_MILLIS();
    timer->isStarted = true;
}


void SoftTimer::stop(const char *name) {
    softTimerID id = isExists(name);
    stop(id);
}

void SoftTimer::stop(softTimerID id) {
    if (!id) {
        return;
    }

    auto *timer = (SoftTimerDef_t *) id;
    timer->isStarted = false;
}

void SoftTimer::process() {
    uint32_t time = SOFT_TIMER_MILLIS();

    int size = list.size();
    if (size <= 0) {
        return;
    }

    SoftTimerDef_t *pItem;
    for (int i = 0; i < size; i++) {
        pItem = list.get(i);
        if (!pItem)
            return;

        if ((pItem->isStarted) && (time - pItem->time > pItem->period)) {
            pItem->callback(pItem->data);
            pItem->time = time;

            if (pItem->mode == stMode_t::stModeOnce) {
                remove(pItem);
            }
        }

    }

}




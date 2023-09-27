//
// Created by moon on 2023/6/15.
//

#include "SoftTimer.h"
#include <string.h>


#if !SOFT_TIMER_DYNAMIC_EN
SoftTimerDef TimerInstance[SOFT_TIMER_MAX_NUM];
#endif

SoftTimer::SoftTimer(SoftTimerGetTime_t _getTimestamp) :
        getTimestamp(_getTimestamp) {
    list.clear();
}


SoftTimer::~SoftTimer() {
    list.clear();
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
softTimerID SoftTimer::create(const char *name, uint32_t period, void (*callback)(void *), void *data, Mode mode) {

    if (isExists(name) || callback == nullptr) {
        // timer is exists.
        return nullptr;
    }

    SoftTimerDef *new_timer = nullptr;

#if SOFT_TIMER_DYNAMIC_EN
    new_timer = new(SoftTimerDef);

    if (nullptr == new_timer) {
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

    new_timer->name      = (char *) name;
    new_timer->period    = (period > 0) ? period : 1;
    new_timer->mode      = mode;
    new_timer->isStarted = false;
    new_timer->callback  = callback;
    new_timer->data      = data;

    list.push_back(new_timer);

    return new_timer;
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

    auto *timer = (SoftTimerDef *) id;

    if (getTimestamp == nullptr)
        return;

    timer->time      = getTimestamp();
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

    auto *timer = (SoftTimerDef *) id;
    timer->isStarted = false;
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
    auto *remove_timer = list[index];
    list.erase(list.begin() + index);

#if SOFT_TIMER_DYNAMIC_EN
    // free the timer memory
    delete remove_timer;
#else
    memset(remove_timer, 0, sizeof(SoftTimerDef));
#endif

}


softTimerID SoftTimer::isExists(const char *name) {
    if (list.empty())
        return nullptr;

    for (auto &item: list) {
        if (strcmp(item->name, name) == 0) {
            return item;
        }
    }

    return nullptr;
}

int SoftTimer::isExists(softTimerID id) {
    if (list.empty())
        return -1;

    int       index = 0;
    for (auto &item: list) {
        if (item == id)
            return index;
        index++;
    }

    return -1;
}

uint32_t SoftTimer::getFreeTime() {
    if (getTimestamp == nullptr)
        return 0;

    uint32_t nowTime  = getTimestamp();
    uint32_t freeTime = UINT32_MAX;

    for (auto &timer: list) {
        if (!timer->isStarted) {
            continue;
        }

        uint32_t time = timer->time + timer->period - nowTime;
        if (time < freeTime)
            freeTime = time;
    }

    return freeTime == UINT32_MAX ? 0 : freeTime;
}


void SoftTimer::process() {
    if (getTimestamp == nullptr)
        return;

    uint32_t time = getTimestamp();

    if (list.empty())
        return;

    for (auto &timer: list) {
        if (timer == nullptr)
            return;

        if (timer->isStarted && (time - timer->time > timer->period)) {
            timer->callback(timer->data);
            timer->time = time;

            if (timer->mode == SoftTimer::OnceMode)
                remove(timer);
        }
    }

}

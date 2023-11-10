//
// Created by moon on 2023/11/10.
//

#include "TimerBase.h"

void TimerBase::start(uint32_t cnt) {
//    if(!getTimestamp)
//        return;
//
    time      = cnt;
    isStarted = true;
}

void TimerBase::stop() {
    isStarted = false;
}

uint32_t TimerBase::calcTimeLeft(uint32_t current_time) {
    uint32_t diff = current_time - time;

    timeLeft = (diff >= period) ? 0 : period - diff;

    return timeLeft;
}

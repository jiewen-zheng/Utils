//
// Created by moon on 2023/11/10.
//

#ifndef STM32PICODK_SOFTTIMER_H
#define STM32PICODK_SOFTTIMER_H

#include <cstdint>
#include "list"

#include "TimerBase.h"

class SoftTimer {
public:
    explicit SoftTimer(TimerBase::GetTime get_time) :
            getTimestamp(get_time) {}

    ~SoftTimer() {}

    void add(TimerBase *timer); //!< static memory method
    TimerBase* create(const char *name, uint32_t period, TimerBase::Callback callback,
                void *data = nullptr, TimerBase::Mode mode = TimerBase::CirculateMode);//!< dynamic memory method

    void remove(TimerBase *timer);
    void remove(const char* name);
    TimerBase *isExists(TimerBase *timer);
    TimerBase *isExists(const char *name);

    void run();

    uint32_t freeTime();

private:
    GetTimerBase::GetTime getTimestamp = nullptr;

    std::list<TimerBase *> list;
};

#endif //STM32PICODK_SOFTTIMER_H

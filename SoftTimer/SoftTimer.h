//
// Created by moon on 2023/6/15.
//

#ifndef SOFT_TIMER_H
#define SOFT_TIMER_H

#include <cstdint>
#include <vector>

/**
 * Enable dynamic creat timer
 */
#define SOFT_TIMER_DYNAMIC_EN   1

/**
 * Static timer max number
 */
#if !SOFT_TIMER_DYNAMIC_EN
#define SOFT_TIMER_MAX_NUM  16
#endif

typedef void *softTimerHandle_t;
typedef softTimerHandle_t softTimerID;

typedef uint32_t(*SoftTimerGetTime_t)();

class SoftTimer {
public:
    /**
 * Timer run mode type
 */
    enum Mode {
        OnceMode = 0,     //!< Timer once run
        CirculateMode    //!< Timer cycle run
    };

    /**
     * Timer object description
     */
    struct SoftTimerDef {
        char     *name;                 //!< Timer name
        uint32_t time;              //!< Current timestamp
        uint32_t period;            //!< Timer period
        Mode     mode;              //!< Timer run mode
        bool     isStarted;             //!< Timer run status
        void (*callback)(void *data);   //!< Timer IRQ callback function
        void *data;                 //!< User defined data object pointer
    };

public:
    explicit SoftTimer(SoftTimerGetTime_t _getTimestamp);
    ~SoftTimer();

    /**
     * Dynamic create timer
     */
    softTimerID create(const char *name, uint32_t period, void(*callback)(void *data),
                       void *data = nullptr, Mode mode = CirculateMode);

    void start(const char *name);
    void start(softTimerID id);
    void stop(const char *name);
    void stop(softTimerID id);

    /**
  * Only the dynamic application timer is available.
  * @param name - Timer name
  */
    void remove(const char *name);
    void remove(softTimerID id);

    softTimerID isExists(const char *name);
    int isExists(softTimerID id);

    uint32_t getFreeTime();

    void process();

private:
    SoftTimerGetTime_t getTimestamp = nullptr;

    std::vector<SoftTimerDef *> list;
};


#endif //SOFT_TIMER_H

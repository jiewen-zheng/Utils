//
// Created by moon on 2023/6/15.
//

#ifndef _SOFT_TIMER_H
#define _SOFT_TIMER_H


#include <stdint.h>
#include "../LinkedList/LinkedList.h"

typedef uint32_t(*SoftTimerGetTime_t)();

/**
 * Provide timer timing
 */
#define SOFT_TIMER_MILLIS stGetTime

/**
 * Enable dynamic creat timer
 */
#define SOFT_TIMER_DYNAMIC_EN   1

/**
 * Static timer max number
 */
#if !SOFT_TIMER_DYNAMIC_EN
#define SOFT_TIMER_MAX_NUM  8
#endif


/**
 * Timer run mode type
 */
typedef enum {
    stModeOnce = 0,     //!< Timer once run
    stModeCirculate,    //!< Timer cycle run
} stMode_t;

/**
 * Timer object description
 */
typedef struct SoftTimerDef_s {
    char *name;                 //!< Timer name
    uint32_t time;              //!< Current timestamp
    uint32_t period;            //!< Timer period
    stMode_t mode;              //!< Timer run mode
    bool isStarted;             //!< Timer run status
    void (*callback)(void *data);   //!< Timer IRQ callback function
    void *data;                 //!< User defined data object pointer
} SoftTimerDef_t;


typedef void *softTimerHandle_t;
typedef softTimerHandle_t softTimerID;

/**
 * Declares a static timer params method
 */
#define softTimerDef(name, period, mode, callback, data) \
const SoftTimerDef_t soft_timer_##name = \
{ #name, 0, period, mode, false, false, callback, data }

/**
 * Get timer object
 */
#define softTimer(name) \
&soft_timer_##name


class SoftTimer {
public:
    SoftTimer() {};

    void setTime(SoftTimerGetTime_t get_Time);

    /**
     * Dynamic create timer
     */
    softTimerID create(const char *name, uint32_t period, void(*callback)(void *data),
                       void *data = nullptr, stMode_t mode = stModeCirculate);

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

    void process();

private:
    SoftTimerGetTime_t stGetTime;

    LinkedList<SoftTimerDef_t *> list;
};


#endif //_SOFT_TIMER_H

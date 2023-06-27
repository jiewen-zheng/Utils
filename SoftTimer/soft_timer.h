//
// Created by Monster on 2023/6/25.
//

#ifndef _SOFT_TIMER_C_H
#define _SOFT_TIMER_C_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "../simple_list/simple_list.h"

typedef uint32_t(*SoftTimerGetTime_t)();

extern SoftTimerGetTime_t stGetTime;

/**
 * Provide timer timing
 */
#define SOFT_TIMER_MILLIS stGetTime

/**
 * Enable dynamic create timer
 */
#define SOFT_TIMER_DYNAMIC_EN  0

/**
 * Static timer max number
 */
#if !SOFT_TIMER_DYNAMIC_EN
#define SOFT_TIMER_MAX_NUM  16
#endif

typedef void *softTimerHandle_t;
typedef softTimerHandle_t softTimerID;

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
    void (*callback)(softTimerID id, void *data);   //!< Timer IRQ callback function
    void *data;                 //!< User defined data object pointer
} SoftTimerDef_t;


softTimerID stCreate(const char *name, uint32_t period, void(*callback)(softTimerID id, void *data),
                     void *data, stMode_t mode);

void stStart(softTimerID id);

void stStop(softTimerID id);

bool stIsRun(const char *name);

void stRemove(softTimerID id);

void stProcess(void);

void stTime(SoftTimerGetTime_t get_time);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif //_SOFT_TIMER_C_H

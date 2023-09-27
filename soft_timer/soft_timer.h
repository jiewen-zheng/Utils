//
// Created by Monster on 2023/6/25.
//

#ifndef SOFT_TIMER_C_H
#define SOFT_TIMER_C_H

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#include "simple_list/simple_list.h"

typedef uint32_t(*SoftTimerGetTime_t)();


/**
 * Enable dynamic create timer
 */
#define SOFT_TIMER_DYNAMIC_EN  1

/**
 * Static timer max number
 */
#if !SOFT_TIMER_DYNAMIC_EN
#define SOFT_TIMER_MAX_NUM  32
#endif

/**
 * Timer run mode type
 */
typedef enum {
    OnceMode = 0,     //!< Timer once run
    CirculateMode    //!< Timer cycle run
}            stMode_t;

/**
 * Timer object description
 */
typedef struct SoftTimerHandle_t {
    char     *name;             //!< Timer name
    uint32_t time;              //!< Current timestamp
    uint32_t period;            //!< Timer period
    stMode_t mode;              //!< Timer run mode
    bool isStarted;             //!< Timer run status
    void     *data;                 //!< User defined data object pointer
    void (*callback)(struct SoftTimerHandle_t* id, void *data);   //!< Timer IRQ callback function
} SoftTimerDef_t;

typedef SoftTimerDef_t* SoftTimerID;


void sTimerInitialize(SoftTimerGetTime_t get_time);
SoftTimerID sTimerCreate(const char *name, uint32_t period,
                     void(*callback)(SoftTimerID id, void *data), void *data, stMode_t mode);

bool sTimerStart(const char *name);
bool stStart(SoftTimerID id);
void sTimerStop(const char *name);
void stStop(SoftTimerID id);
bool sTimerIsRun(const char *name);
void sTimerRemove(SoftTimerID id);
uint32_t sTimerFreeTime();
void sTimerProcess(void);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif //SOFT_TIMER_C_H

//
// Created by Monster on 2023/6/25.
//

#include "soft_timer.h"
#include "string.h"
#include "stdlib.h"

SoftTimerGetTime_t stGetTime;

#if !SOFT_TIMER_DYNAMIC_EN
SoftTimerDef_t SoftTimer[SOFT_TIMER_MAX_NUM];
#endif

/**
 * \brief define timer list global variable
 */
SimpleListDef(softimer)
        List_t *stList = GetList(softimer);
        ListMethod_t *stMethod = GetMethod(softimer);

/**
 * \brief check the timer exists
 * \param id timer handle
 * \return index of the timer in the list,
 *          no timer return -1.
 */
static int isExists(SoftTimerID id) {
    int size = stMethod->size(stList);

    for (int i = 0; i < size; i++) {
        SoftTimerID pItem = (SoftTimerID) stMethod->get(stList, i);
        if (pItem == id) {
            return i;
        }
    }

    return -1;
}

static SoftTimerID getTimer(const char *name) {
    int size = stMethod->size(stList);

    for (int i = 0; i < size; i++) {
        SoftTimerDef_t *pItem = (SoftTimerDef_t *) stMethod->get(stList, i);
        if (strcmp(pItem->name, name) == 0) {
            return pItem;
        }
    }

    return NULL;
}

/**
 * \brief initialize soft timer, should be before using timer.
 * \param get_time get timestamp function pointer
 */
void sTimerInitialize(SoftTimerGetTime_t get_time) {
    SimpleListInit(softimer)
    stGetTime = get_time;
}

/**
 * \brief create timer
 * \param name timer name
 * \param period timer time period
 * \param callback timeout callback function pointer.
 * \param data callback function params pointer.
 * \param mode timer run mode used "OnceMode" or "CirculateMode".
 * \return the timer handle
 */
SoftTimerID sTimerCreate(const char *name, uint32_t period, void(*callback)(SoftTimerID id, void *data),
                         void *data, stMode_t mode) {

    if (name == NULL) {
        return NULL;
    }

    if (callback == NULL || getTimer((char *) name)) {
        return NULL;
    }

    SoftTimerDef_t *timer = NULL;

#if SOFT_TIMER_DYNAMIC_EN
    timer = (SoftTimerDef_t *) malloc(sizeof(SoftTimerDef_t));
    if (timer == NULL) {
        return NULL;
    }
#else
    for (int i = 0; i < SOFT_TIMER_MAX_NUM; i++) {
        if (SoftTimer[i].name == NULL) {
            timer = &SoftTimer[i];
            break;
        }
    }

    /* timer create upper limit */
    if (timer == NULL) {
        return NULL;
    }
#endif
    memset(timer, 0, sizeof(SoftTimerDef_t));

    timer->name      = (char *) name;
    timer->time      = stGetTime();
    timer->period    = period;
    timer->mode      = mode;
    timer->isStarted = false;
    timer->callback  = callback;
    timer->data      = data;

    if (!stMethod->add(stList, timer)) {
#if SOFT_TIMER_DYNAMIC_EN
        free(timer);
#endif
        return NULL;
    }

    return timer;
}

bool sTimerStart(const char *name) {
    if (name == NULL) {
        return false;
    }

    SoftTimerID id = getTimer(name);

    return stStart(id);
}

bool stStart(SoftTimerID id) {
    if (!id) {
        return false;
    }

    SoftTimerDef_t *timer = (SoftTimerDef_t *) id;

    timer->time      = stGetTime();
    timer->isStarted = true;

    return true;
}

void stStop(SoftTimerID id) {
    if (!id) {
        return;
    }
    SoftTimerDef_t *timer = (SoftTimerDef_t *) id;
    timer->isStarted = false;
}

bool sTimerIsRun(const char *name) {
    SoftTimerID id = getTimer((char *) name);
    if (!id) {
        return false;
    }

    SoftTimerDef_t *timer = (SoftTimerDef_t *) id;
    return timer->isStarted;
}

void sTimerRemove(SoftTimerID id) {
    int index = isExists(id);
    if (index == -1) {
        return;
    }

    // remove list none
    SoftTimerDef_t *remove_timer = (SoftTimerDef_t *) stMethod->remove(stList, index);

#if SOFT_TIMER_DYNAMIC_EN
    // free the timer memory
    free(remove_timer);
#else
    // clear array
    memset(remove_timer, 0, sizeof(SoftTimerDef_t));
#endif
}

/**
 * \brief Gets the minimum remaining timeout time in the timer.
 * \return remain timeout.
 */
uint32_t sTimerFreeTime() {
    int size = stMethod->size(stList);
    if (size <= 0) {
        return 0;
    }

    uint32_t       nowTime  = stGetTime();
    uint32_t       freeTime = UINT32_MAX;
    SoftTimerDef_t *timer;

    for (int i = 0; i < size; i++) {
        timer = (SoftTimerDef_t *) stMethod->get(stList, i);
        if (!timer)
            continue;

        if (!timer->isStarted)
            continue;

        uint32_t time = timer->time + timer->period - nowTime;
        if (time < freeTime)
            freeTime = time;
    }

    return freeTime == UINT32_MAX ? 0 : freeTime;
}

/**
 * \brief Universal timer processing.
 * He should be called in a loop,
 */
void sTimerProcess(void) {
    uint32_t time = stGetTime();

    int size = stMethod->size(stList);
    if (size <= 0) {
        return;
    }

    SoftTimerDef_t *pItem;

    for (int i = 0; i < size; i++) {
        pItem = (SoftTimerDef_t *) stMethod->get(stList, i);
        if (!pItem)
            continue;

        if (!pItem->isStarted)
            continue;

        /* goto user callback function */
        if (time - pItem->time > pItem->period) {
            pItem->time = time;
            pItem->callback(pItem, pItem->data);

            /* remove once mode timer */
            if (pItem->mode == OnceMode) {
                sTimerRemove(pItem);
            }
        }

    }

}


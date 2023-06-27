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

SimpleListDef(stList);
List_t *stList = SimpleList(stList);
ListMethod_t *stMethod = SimpleListMethod(stList);
static bool list_init = false;


int stIsExists(softTimerID id) {
    int size = stMethod->size(stList);
    for (int i = 0; i < size; i++) {
        softTimerID *pItem = (softTimerID *) stMethod->get(stList, i);
        if (pItem == id) {
            return i;
        }
    }

    return -1;
}

softTimerID checkName(char *name) {
    int size = stMethod->size(stList);
    for (int i = 0; i < size; i++) {
        SoftTimerDef_t *pItem = (SoftTimerDef_t *) stMethod->get(stList, i);
        if (strcmp(pItem->name, name) == 0) {
            return pItem;
        }
    }

    return NULL;
}


softTimerID stCreate(const char *name, uint32_t period, void(*callback)(softTimerID id, void *data),
                     void *data, stMode_t mode) {

    if (!list_init) {
        list_init = true;
        SimpleListInit(stList);
    }

    if (name == NULL) {
        return NULL;
    }

    if (callback == NULL || checkName((char *) name)) {
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

    timer->name = (char *) name;
    timer->time = SOFT_TIMER_MILLIS();
    timer->period = period;
    timer->mode = mode;
    timer->isStarted = false;
    timer->callback = callback;
    timer->data = data;

    if (!stMethod->add(stList, timer)) {
#if SOFT_TIMER_DYNAMIC_EN
        free(timer);
#endif
        return NULL;
    }

    return timer;
}

void stStart(softTimerID id) {
    if (!id) {
        return;
    }

    SoftTimerDef_t *timer = (SoftTimerDef_t *) id;

    timer->time = SOFT_TIMER_MILLIS();
    timer->isStarted = true;
}

void stStop(softTimerID id) {
    if (!id) {
        return;
    }
    SoftTimerDef_t *timer = (SoftTimerDef_t *) id;
    timer->isStarted = false;
}

bool stIsRun(const char *name) {
    softTimerID id = checkName((char *) name);
    if (!id) {
        return false;
    }

    SoftTimerDef_t *timer = (SoftTimerDef_t *) id;
    return timer->isStarted;
}

void stRemove(softTimerID id) {
    int index = stIsExists(id);
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

void stProcess(void) {
    uint32_t time = SOFT_TIMER_MILLIS();

    int size = stMethod->size(stList);
    if (size <= 0) {
        return;
    }

    SoftTimerDef_t *pItem;
    for (int i = 0; i < size; i++) {
        pItem = (SoftTimerDef_t *) stMethod->get(stList, i);
        if (!pItem)
            return;

        /* goto user callback function */
        if ((pItem->isStarted) && (time - pItem->time > pItem->period)) {
            pItem->callback(pItem, pItem->data);
            pItem->time = time;

            /* remove once mode timer */
            if (pItem->mode == stModeOnce) {
                stRemove(pItem);
            }
        }

    }

}

void stTime(SoftTimerGetTime_t get_time) {
    stGetTime = get_time;
}


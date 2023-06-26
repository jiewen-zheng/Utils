//
// Created by Monster on 2023/6/25.
//
#include "../../soft_timer.h"
#include "stdio.h"

softTimerID timer1;
softTimerID timer4;

uint32_t get_time() {
    static uint32_t cnt = 0;
    return cnt++;
}

void timer_cb(softTimerID id, void *data) {
    SoftTimerDef_t *timer = (SoftTimerDef_t *) id;

    if (timer->name == "timer1") {
        printf("timer1 \r\n");
        stStop(id);
    } else if (timer->name == "timer2") {
        printf("timer2 \r\n");
        timer4 = stCreate("timer4", 200, timer_cb, NULL, stModeCirculate);
        stStart(timer4);
    } else if (timer->name == "timer3") {
        printf("timer3 \r\n");
        if (!stIsRun("timer1")) {
            stStart(timer1);
        }
    } else if (timer->name == "timer4") {
        printf("timer4 \r\n");
    }

}

int main_app() {

    stTime(get_time);
    timer1 = stCreate("timer1", 100, timer_cb, NULL, stModeCirculate);
    stStart(timer1);
    softTimerID timer2 = stCreate("timer2", 200, timer_cb, NULL, stModeOnce);
    stStart(timer2);
    softTimerID timer3 = stCreate("timer3", 300, timer_cb, NULL, stModeCirculate);
    stStart(timer3);

    for (;;) {
        stProcess();
    }
}


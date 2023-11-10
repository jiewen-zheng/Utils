//
// Created by moon on 2023/6/17.
//
#include "../../SoftTimer.h"
#include "stdint.h"

uint32_t millis() {
    return 0;
}


SoftTimer softTimer(millis);


void timer_callback(void *data){

    TimerBase *timer = (TimerBase*) data;
    if(timer->name == "t1"){

    }else if(timer->name == "t2"){

    }
}

TimerBase t2("t2", 500, timer_callback);

void setup() {
    //!< dynamic create
    TimerBase *t1 = softTimer.create("t1", 1000, timer_callback);

    //!< static create
    softTimer.add(&t2);

    t1->start(0);
    t2.start();
}

void loop(){
    softTimer.run();
}
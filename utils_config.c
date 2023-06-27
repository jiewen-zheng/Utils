//
// Created by moon on 2023/6/26.
//

#include "utils_config.h"
#include "delay.h"

uint32_t millis(void) {
    return Drv_GetMillis();
}

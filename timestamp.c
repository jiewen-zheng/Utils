//
// Created by moon on 2023/6/26.
//

#include "timestamp.h"
#include "stm32f1xx_hal.h"

uint32_t millis() {
    return HAL_GetTick();
}

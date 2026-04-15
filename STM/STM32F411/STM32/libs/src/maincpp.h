#ifndef MAINCPP_H
#define MAINCPP_H

#ifdef __cplusplus
extern "C" {
#endif

#pragma once
#include "stm32f4xx_hal.h"

extern UART_HandleTypeDef huart2;
//extern I2C_HandleTypeDef hi2c1;
void setup();
void loop(void* params);

#ifdef __cplusplus
}
#endif

#endif // MAINCPP_H
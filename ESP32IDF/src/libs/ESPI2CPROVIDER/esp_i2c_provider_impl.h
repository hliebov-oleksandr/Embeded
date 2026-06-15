#pragma once
/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "i2cprovider.h"

void checkDevice();
void INITIALIZE_I2C(int clockPin, int dataPin);

/* C++ detection */
#ifdef __cplusplus
}
#endif
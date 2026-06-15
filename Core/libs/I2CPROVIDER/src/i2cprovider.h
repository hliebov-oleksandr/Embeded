#pragma once

/* C++ detection */
#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"
#include "string.h"
#include <stdbool.h>

typedef bool (*I2C_Master_TransmitProvider)(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
typedef bool (*I2C_IsDeviceReadyProvider)(uint16_t DevAddress, uint32_t Trials, uint32_t Timeout);

typedef struct 
{  
    I2C_Master_TransmitProvider I2C_Master_Transmit;
    I2C_IsDeviceReadyProvider I2C_IsDeviceReady;
} I2CProvider;

I2CProvider GET_I2C_PROVIDER();

#ifdef __cplusplus
}
#endif

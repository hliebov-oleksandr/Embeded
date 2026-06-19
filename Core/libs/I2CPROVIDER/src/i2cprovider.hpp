#pragma once

#include "stdlib.h"
#include "string.h"
#include <stdbool.h>
#include "i2cproviderdata.h"

class I2CProvider {
    public:
        virtual bool IC2CTransmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout) = 0;
        virtual bool I2CIsReady(uint16_t DevAddress, uint32_t Trials, uint32_t Timeout) = 0;
        virtual void CheckI2CDevices() = 0;        
};

#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "driver/i2c.h"
#include "driver/i2c_master.h"
#include "freertos/FreeRTOS.h"
#include "i2cprovider.hpp"

class I2CESPProvider : public I2CProvider
{
private:
    gpio_num_t clockPin;
    gpio_num_t dataPin;
    uint16_t address;
    i2c_port_num_t port;

    i2c_master_bus_handle_t bus_handle;
    i2c_master_dev_handle_t dev_handle;

    
public:
    I2CESPProvider(gpio_num_t clockPin, gpio_num_t dataPin, uint16_t address, i2c_port_num_t port);
    //I2CHandlers_t GetI2CProvider();
    bool IC2CTransmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    bool I2CIsReady(uint16_t DevAddress, uint32_t Trials, uint32_t Timeout);
    void CheckI2CDevices();
};

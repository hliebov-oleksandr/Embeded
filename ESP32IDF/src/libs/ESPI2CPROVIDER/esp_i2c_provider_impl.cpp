#include "esp_i2c_provider_impl.hpp"
#include "esp_err.h"
#include "driver/i2c_master.h"

#define I2C_PORT I2C_NUM_0
#define ADDRESS_OLED 0x3C

 
void I2CESPProvider::CheckI2CDevices()
{
    for (int addr = 0x08; addr < 0x78; addr++)
    {
        if (i2c_master_probe(bus_handle, addr, pdMS_TO_TICKS(100)) == ESP_OK)
        {
            printf("Found device at 0x%02X\n", addr);
        }
    }
}

I2CESPProvider::I2CESPProvider(gpio_num_t clockPin, gpio_num_t dataPin, uint16_t address):
        clockPin(clockPin), dataPin(dataPin),address(address)
{
    
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_PORT,
        .sda_io_num = dataPin,
        .scl_io_num = clockPin,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7        
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    i2c_device_config_t dev_cfg = {
        .device_address = address,
        .scl_speed_hz = 400000,
    };

    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
}

bool I2CESPProvider::IC2CTransmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{    
    esp_err_t ret = i2c_master_transmit(dev_handle, pData, Size, pdMS_TO_TICKS(Timeout));
    return ret == ESP_OK;
}

bool I2CESPProvider::I2CIsReady(uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
    for (uint32_t i = 0; i < Trials; i++)
    {
        esp_err_t ret = i2c_master_probe(bus_handle, DevAddress, pdMS_TO_TICKS(Timeout));
        if (ret == ESP_OK)
        {
            return true;
        }
    }
    return false;
}

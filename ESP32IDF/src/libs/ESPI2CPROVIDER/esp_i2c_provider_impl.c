#include "esp_i2c_provider_impl.h"
#include "esp_err.h"
#include "driver/i2c_master.h"

#define I2C_PORT I2C_NUM_0

static i2c_master_bus_handle_t bus_handle;
static i2c_master_dev_handle_t dev_handle;

void checkDevice() {
    for (int addr = 0x08; addr < 0x78; addr++) {
    if (i2c_master_probe(bus_handle, addr, pdMS_TO_TICKS(100)) == ESP_OK) {
        printf("Found device at 0x%02X\n", addr);
    }
}
}

void INITIALIZE_I2C(int clockPin, int dataPin) {
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_PORT,
        .sda_io_num = dataPin,
        .scl_io_num = clockPin,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &bus_handle));

    i2c_device_config_t dev_cfg = {
        .device_address = 0x3C,       // SSD1306 адрес (7‑битный)
        .scl_speed_hz = 400000,       // скорость 400 кГц
    };


    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));
  
}

bool esp_i2c_transmit(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout) {
    uint8_t addr7bit = DevAddress >> 1;
    esp_err_t ret = i2c_master_transmit(dev_handle, pData, Size, pdMS_TO_TICKS(Timeout));
    return ret == ESP_OK;
}

bool esp_i2c_is_ready(uint16_t DevAddress, uint32_t Trials, uint32_t Timeout) {
    uint8_t addr7bit = DevAddress >> 1;    
    printf("ready a + : %d\n", addr7bit);
    for (uint32_t i = 0; i < Trials; i++) {
        esp_err_t ret = i2c_master_probe(bus_handle, addr7bit, pdMS_TO_TICKS(Timeout));
        if (ret == ESP_OK) {
            return true;
        }
    }
    return false;
}

I2CProvider GET_I2C_PROVIDER() {
    I2CProvider espI2CProvider;
    espI2CProvider.I2C_Master_Transmit = esp_i2c_transmit;
    espI2CProvider.I2C_IsDeviceReady = esp_i2c_is_ready;
    return espI2CProvider;
}

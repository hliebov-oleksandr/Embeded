#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "libs/GPIO/espidfgpio.h"
#include "encoder.h"
#include "driver/i2c.h"

#define ENCODER_A_GPIO 4
#define ENCODER_B_GPIO 5

#define DISPLAY_CLCK_GPIO 20
#define DISPLAY_DATA_GPIO 21

void vReadEncoderTask(void *params)
{

    EspIDFGPIO gpioA(gpio_num_t::GPIO_NUM_4, gpio_mode_t::GPIO_MODE_INPUT);
    EspIDFGPIO gpioB(gpio_num_t::GPIO_NUM_5, gpio_mode_t::GPIO_MODE_INPUT);

    gpioA.pullUpDown(PulUPDonw::PULL_UP);
    gpioB.pullUpDown(PulUPDonw::PULL_UP);
    Encoder encoder(&gpioA, &gpioB);

    while (true)
    {
        EncoderResult result = encoder.readEncoder();
        if (result.encoderChanged)
        {
            printf("Encoder C: %d \n", result.counter);
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void vDisplayTask(void *pvParameters)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = DISPLAY_CLCK_GPIO,
        .scl_io_num = DISPLAY_DATA_GPIO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master{
            .clk_speed = 100000}};

    i2c_param_config(I2C_NUM_0, &conf);     
    i2c_driver_install(I2C_NUM_0, conf.mode, 0, 0, 0);

    char buffer[64];
/*
    for (;;)
    {
        if (xQueueReceive(xDisplayQueue, &buffer, portMAX_DELAY) == pdPASS)
        {
            ssd1306_clear(&dev);
            ssd1306_draw_string(&dev, 0, 0, buffer, 12, true);
            ssd1306_refresh(&dev);
        }
    }
        */
}

extern "C" void app_main(void)
{
    xTaskCreate(vReadEncoderTask, "ReadEncoder", 4056, NULL, 1, NULL);
    xTaskCreate(vDisplayTask, "DisplayTask", 4056, NULL, 1, NULL);
    
    while (true)
    {
        vTaskDelay(pdMS_TO_TICKS(2));
    }
}
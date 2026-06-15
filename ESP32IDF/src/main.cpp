#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "libs/GPIO/espidfgpio.h"
#include "encoder.h"
#include "libs/ESPI2CPROVIDER/esp_i2c_provider_impl.h"
#include "ssd1306.h"

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
    vTaskDelay(pdMS_TO_TICKS(20));
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
    
    INITIALIZE_I2C(20, 21);

    I2CProvider i2cProvider = GET_I2C_PROVIDER();
    SSD_INITIALIZE_I2C_PROVIDER(i2cProvider);
    SSD1306_Init();
    SSD1306_GotoXY(0, 0);

    SSD1306_GotoXY(0, 30);
    //SSD1306_Puts("TIME 12:23:00", &Font_11x18, SSD1306_COLOR_t::SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();

    //
    xTaskCreate(vReadEncoderTask, "ReadEncoder", 4056, NULL, 1, NULL);
    //xTaskCreate(vDisplayTask, "DisplayTask", 4056, NULL, 1, NULL);
    
    while (true)
    {     
        checkDevice();   
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
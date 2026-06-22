#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "libs/GPIO/espidfgpio.h"
#include "encoder.h"
#include "libs/ESPI2CPROVIDER/esp_i2c_provider_impl.hpp"
#include "ssd1306.h"
#include "esp_timer.h"
#include "menu.hpp"

#define ENCODER_A_GPIO 4
#define ENCODER_B_GPIO 5
#define ENCODER_CLICK_GPIO 6

#define DISPLAY_CLCK_GPIO gpio_num_t::GPIO_NUM_21 // 21
#define DISPLAY_DATA_GPIO gpio_num_t::GPIO_NUM_47 // 47
#define DISPLA_I2C_ADDRESS 0x3C
#define I2C_PORT I2C_NUM_0

QueueHandle_t encoderQueue;


void enable_wifi()
{
    printf("\nEnable wo - fi");
}

void settings()
{
    printf("\nsettings");
}

void lock()
{

}

const int MENU_COUNT = 5;
menu_item_t main_menu[MENU_COUNT] = {
    {1, "enable wi-fi", enable_wifi, NULL, 0},
    {2, "settings", settings, NULL, 0},
    {3, "lock", lock, NULL, 0},
    {4, "time", NULL, NULL, 0},
    {5, "exit", NULL, NULL, 0}};
Menu menu(main_menu, MENU_COUNT);

I2CESPProvider i2CLed(DISPLAY_CLCK_GPIO, DISPLAY_DATA_GPIO, DISPLA_I2C_ADDRESS, I2C_PORT);

bool IC2CLedTransmitWrapper(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    return i2CLed.IC2CTransmit(DevAddress, pData, Size, Timeout);
}

bool IC2CReadytWrapper(uint16_t DevAddress, uint32_t Trials, uint32_t Timeout)
{
    return i2CLed.I2CIsReady(DevAddress, Trials, Timeout);
}

void draw_menu(EncoderResult result)
{
    if (result.resultType == ENCODER_RESULT_TYPE::CHANGE_DIRECTION)
    {
        if (result.direction == ENCODER_DIRECTION_TYPE::RIGHT_INCREASE)
        {
            menu.up();
        }
        else if (result.direction == ENCODER_DIRECTION_TYPE::LEFT_DICREASE)
        {
            menu.dowm();
        }
    }
    else if (result.resultType == ENCODER_RESULT_TYPE::CLICKED)
    {        
        menu.enter();
    }  
}

void vReadEncoderTask(void *params)
{
    EspIDFGPIO gpioA(gpio_num_t::GPIO_NUM_4, gpio_mode_t::GPIO_MODE_INPUT);
    EspIDFGPIO gpioB(gpio_num_t::GPIO_NUM_5, gpio_mode_t::GPIO_MODE_INPUT);
    EspIDFGPIO gpioClick(gpio_num_t::GPIO_NUM_6, gpio_mode_t::GPIO_MODE_INPUT);

    gpioA.pullUpDown(PulUPDonw::PULL_UP);
    gpioB.pullUpDown(PulUPDonw::PULL_UP);

    Encoder encoder(&gpioA, &gpioB, &gpioClick, esp_timer_get_time);

    while (true)
    {
        EncoderResult result = encoder.readEncoder();
        if (result.encoderChanged)
        {
            printf("Encoder C: %d \n", result.counter);
            xQueueSend(encoderQueue, &result, portMAX_DELAY);
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }
}

void vDisplayTask(void *pvParameters)
{
    EncoderResult recvResult;
    while (true)
    {
        if (xQueueReceive(encoderQueue, &recvResult, portMAX_DELAY) == pdTRUE)
        {
            draw_menu(recvResult);
        }
    }
}

extern "C" void app_main(void)
{
    encoderQueue = xQueueCreate(10, sizeof(EncoderResult));
    I2CHandlers_t i2CLedHandlers = {
        .I2C_Master_Transmit = IC2CLedTransmitWrapper,
        .I2C_IsDeviceReady = IC2CReadytWrapper,
        .Address = DISPLA_I2C_ADDRESS};
    SSD1306_Init_I2C_Provider(i2CLedHandlers);
    menu.drawMenu();
    //
    xTaskCreate(vReadEncoderTask, "ReadEncoder", 4096, NULL, 1, NULL);
    xTaskCreate(vDisplayTask, "DisplayTask", 4096, NULL, 1, NULL);
    //while (true){//i2CLed.CheckI2CDevices();//vTaskDelay(pdMS_TO_TICKS(2000));}
}
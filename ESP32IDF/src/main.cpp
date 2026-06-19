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

#define ENCODER_A_GPIO 4
#define ENCODER_B_GPIO 5
#define ENCODER_CLICK_GPIO 6

#define DISPLAY_CLCK_GPIO gpio_num_t::GPIO_NUM_21 // 21
#define DISPLAY_DATA_GPIO gpio_num_t::GPIO_NUM_47 //47
#define DISPLA_I2C_ADDRESS  0x3C

QueueHandle_t encoderQueue;

struct menu_item_t{
    int8_t id;
    char* name;    
    void (*action_h)();
    struct menu_item_t* sub_menu;
    int8_t sub_menu_length;
};

typedef struct menu_item_t;

void enable_wifi() {
    printf("Enable wo - fi");
}

void settings(){
    printf("settings");
}

void log() {

}

const int MENU_COUNT = 5;

menu_item_t main_menu[MENU_COUNT] = {
  { 1, "enable wi-fi", enable_wifi, NULL, 0},
  { 2, "settings", settings, NULL, 0},  
  { 3, "log", log, NULL, 0 },
  { 4, "time", log, NULL, 0},
  { 5, "exit", log, NULL, 0}
};

int current_menu_id = 0;

void draw_menu(EncoderResult result) {
    SSD1306_Clear();
    if (current_menu_id > MENU_COUNT) current_menu_id = 0;
    if (current_menu_id < 0) current_menu_id = MENU_COUNT;

    current_menu_id = current_menu_id + (result.direction == ENCODER_DIRECTION_TYPE::RIGHT_INCREASE ? 1 : -1);
    for (int i = 0; i < MENU_COUNT; i++) {                  
        if (current_menu_id == i + 1) {            
            SSD1306_GotoXY(0, (Font_7x10.FontHeight * i) + 2);
            SSD1306_Puts("> ", &Font_7x10, SSD1306_COLOR_t::SSD1306_COLOR_WHITE);                        
        }
        SSD1306_GotoXY(Font_7x10.FontWidth + 1, (Font_7x10.FontHeight * i) + 2);
        SSD1306_Puts(main_menu[i].name, &Font_7x10, SSD1306_COLOR_t::SSD1306_COLOR_WHITE);                    
    }
    SSD1306_UpdateScreen();    
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

I2CESPProvider i2CLed(DISPLAY_CLCK_GPIO, DISPLAY_DATA_GPIO, DISPLA_I2C_ADDRESS);

bool IC2CLedTransmitWrapper(uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout) { 
 return i2CLed.IC2CTransmit(DevAddress, pData, Size, Timeout);
}

bool IC2CReadytWrapper(uint16_t DevAddress, uint32_t Trials, uint32_t Timeout) { 
 return i2CLed.I2CIsReady(DevAddress, Trials, Timeout);
}

extern "C" void app_main(void)
{
    encoderQueue = xQueueCreate(10, sizeof(EncoderResult));
    I2CHandlers_t i2CLedHandlers = {
        .I2C_Master_Transmit = IC2CLedTransmitWrapper,
        .I2C_IsDeviceReady = IC2CReadytWrapper,
        .Address = DISPLA_I2C_ADDRESS
    };
    SSD1306_Init_I2C_Provider(i2CLedHandlers);        
        
    //
    xTaskCreate(vReadEncoderTask, "ReadEncoder", 4096, NULL, 1, NULL);
    xTaskCreate(vDisplayTask, "DisplayTask", 4096, NULL, 1, NULL);
    
    while (true) {  i2CLed.CheckI2CDevices();   vTaskDelay(pdMS_TO_TICKS(2000));}
}
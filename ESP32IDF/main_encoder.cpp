#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/pulse_cnt.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "libs/GPIO/espidfgpio.h"
#include "encoder.h"

#define ENCODER_A_GPIO 4
#define ENCODER_B_GPIO 5


void readEncoder(void* params) {

    EspIDFGPIO gpioA(gpio_num_t::GPIO_NUM_4, gpio_mode_t::GPIO_MODE_INPUT);
    EspIDFGPIO gpioB(gpio_num_t::GPIO_NUM_5, gpio_mode_t::GPIO_MODE_INPUT);
    gpioA.pullUpDown(PulUPDonw::PULL_UP);
    gpioB.pullUpDown(PulUPDonw::PULL_UP);    
    Encoder encoder(&gpioA, &gpioB);    

    while (true)
    {    
        EncoderResult result = encoder.readEncoder();
        if (result.encoderChanged) {
            printf("Encoder C: %d \n", result.counter);            
        }
        vTaskDelay(pdMS_TO_TICKS(5));
    }        
}

extern "C" void app_main(void)
{
    EspIDFGPIO gpioA(gpio_num_t::GPIO_NUM_4, gpio_mode_t::GPIO_MODE_INPUT);
    EspIDFGPIO gpioB(gpio_num_t::GPIO_NUM_5, gpio_mode_t::GPIO_MODE_INPUT);
    gpioA.pullUpDown(PulUPDonw::PULL_UP);
    gpioB.pullUpDown(PulUPDonw::PULL_UP);    
    Encoder encoder(&gpioA, &gpioB);
    xTaskCreate(readEncoder, "ReadEncoder", 4056, NULL, 1, NULL);        
    while (true){ vTaskDelay(pdMS_TO_TICKS(2)); }
}
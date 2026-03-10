#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "gpiopwm.h"
#include "gpio.h"
#include "led.h"
#include "libs/GPIO/espidfgpio.h"
#include "traficlight.h"

 
unsigned long getMicros();

class LedWorkConfig
{
public:
  static constexpr gpio_num_t LED_PIN_GREEN = GPIO_NUM_10; //10;
  static constexpr gpio_num_t LED_PIN_YELLOW = GPIO_NUM_13; //13;
  static constexpr gpio_num_t LED_PIN_RED = GPIO_NUM_11; //11;
  static constexpr gpio_num_t PWWPOTENCIOMETR_PIN = GPIO_NUM_12; //12;

  static constexpr int PWW_LED_FREQUENCY_GZ = 1000;
  static constexpr int ANALOG_RESOLUTION = 4095;
};

EspIDFGPIO lightPotenciometr(LedWorkConfig::PWWPOTENCIOMETR_PIN, true);
EspIDFGPIO greenGPIO(LedWorkConfig::LED_PIN_GREEN, GPIO_MODE_OUTPUT);
EspIDFGPIO yellowGPIO(LedWorkConfig::LED_PIN_YELLOW, GPIO_MODE_OUTPUT);
EspIDFGPIO redGPIO(LedWorkConfig::LED_PIN_RED, GPIO_MODE_OUTPUT);

unsigned long getMicros() {
    return (unsigned long) (esp_timer_get_time());
}

GPIOPWMControl pwwControlGreen(&greenGPIO, LedWorkConfig::PWW_LED_FREQUENCY_GZ, getMicros);
GPIOPWMControl pwwControlYellow(&yellowGPIO, LedWorkConfig::PWW_LED_FREQUENCY_GZ, getMicros);
GPIOPWMControl pwwControlRed(&redGPIO, LedWorkConfig::PWW_LED_FREQUENCY_GZ, getMicros);

Led ledGreen(&pwwControlGreen);
Led ledYellow(&pwwControlYellow);
Led ledRed(&pwwControlRed);

TraficWorkConfig traficConfig = {
    .GREEN_LIGHT_TIME_MILISEC = 6000,
    .GREEN_BLINK_LIGHT_TIME_MILISEC = 2000,
    .YELLOW_LIGHT_TIME_MILISEC = 2000,
    .RED_LIGHT_MILISEC = 6000,
    .RED_YELLOW_LIGHT_MILISEC = 2000 
};

TraficLightController traficController(&ledGreen, &ledYellow, &ledRed, traficConfig);
volatile long period_count_milisec = 0;

void IRAM_ATTR onTimer()
{
  period_count_milisec = period_count_milisec + 1;
  if (period_count_milisec >= traficController.getTraficAllWorkPeriod())
  {
    period_count_milisec = 0;
  }
}


void readLightingTraficSensorTask(void* params) {
    while (true)
    {
        vTaskDelay(1);
        onTimer();        
    }    
}

void startTraficLight(void *params) {
    while (true)
    {        
        int pwwValue = lightPotenciometr.analogReqad();// 2544; //analogRead(LedWorkConfig::PWWPOTENCIOMETR_PIN);   
        //printf("pww: %d", pwwValue); vTaskDelay(1000); 
        pwwControlGreen.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
        pwwControlYellow.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
        pwwControlRed.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
        traficController.LightTrafic(period_count_milisec);        
    }   
}

extern "C" void app_main() {    
    //TaskHandle_t *potenciometTaskHandler;    
    printf("Hello world ");
    xTaskCreate(readLightingTraficSensorTask, "readLightingTraficSensorTask", 2048, NULL, 1, NULL);
    xTaskCreate(startTraficLight, "startTraficLight", 2048, NULL, 1, NULL);
    
    int i = 0;
    while (true)
    {
        int pwwValue = lightPotenciometr.analogReqad();// 2544; //analogRead(LedWorkConfig::PWWPOTENCIOMETR_PIN);   
        printf("pww: %d", pwwValue); //vTaskDelay(1000); 
        vTaskDelay(1000);
        //printf("main thread %d \n", (++i));
    }    
}
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
#include "driver/ledc.h"

#define SERVO_GPIO 4

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0

#define LEDC_DUTY_RES LEDC_TIMER_10_BIT
#define SERVO_PWM_FREQ 50
#define SERVO_PERIOD_US 2000

#define SERVO_MIN_US 500
#define SERVO_MAX_US 2400

static void servo_init(void)
{
    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,        
        .freq_hz = SERVO_PWM_FREQ,
        .clk_cfg = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer_config));
    ledc_channel_config_t channel_config = {
        .gpio_num = SERVO_GPIO,
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .intr_type = LEDC_INTR_DISABLE,        
        .timer_sel = LEDC_TIMER,        
        .duty = 0,
        .hpoint = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel_config));
};

unsigned long getMicros();
class LedWorkConfig
{
public:
    static constexpr gpio_num_t LED_PIN_GREEN = GPIO_NUM_10;       // 10;
    static constexpr gpio_num_t LED_PIN_YELLOW = GPIO_NUM_13;      // 13;
    static constexpr gpio_num_t LED_PIN_RED = GPIO_NUM_11;         // 11;
    static constexpr gpio_num_t PWWPOTENCIOMETR_PIN = GPIO_NUM_12; // 12;

    static constexpr int PWW_LED_FREQUENCY_GZ = 1000;
    static constexpr int ANALOG_RESOLUTION = 4095;
};

EspIDFGPIO lightPotenciometr(LedWorkConfig::PWWPOTENCIOMETR_PIN, true);
EspIDFGPIO greenGPIO(LedWorkConfig::LED_PIN_GREEN, GPIO_MODE_OUTPUT);
EspIDFGPIO yellowGPIO(LedWorkConfig::LED_PIN_YELLOW, GPIO_MODE_OUTPUT);
EspIDFGPIO redGPIO(LedWorkConfig::LED_PIN_RED, GPIO_MODE_OUTPUT);

unsigned long getMicros()
{
    return (unsigned long)(esp_timer_get_time());
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
    .RED_YELLOW_LIGHT_MILISEC = 2000};

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

void readLightingTraficSensorTask(void *params)
{
    while (true)
    {
        vTaskDelay(1);
        onTimer();
    }
}

void startTraficLight(void *params)
{
    while (true)
    {
        if (traficController.getCurrentMode(period_count_milisec) == GREEN_BLINK_LIGHT)
        {
            vTaskDelay(10);
        }

        int pwwValue = 2544; // lightPotenciometr.analogReqad();//
        // printf("pww: %d", pwwValue); vTaskDelay(1000);
        pwwControlGreen.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
        pwwControlYellow.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
        pwwControlRed.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
        traficController.LightTrafic(period_count_milisec);
    }
}

extern "C" void app_main()
{
    // TaskHandle_t *potenciometTaskHandler;
    printf("Hello world ");
    xTaskCreate(readLightingTraficSensorTask, "readLightingTraficSensorTask", 2048, NULL, 1, NULL);
    //xTaskCreate(startTraficLight, "startTraficLight", 2048, NULL, 1, NULL);

    servo_init();
    vTaskDelay(1000);

    int i = 0;
    while (true)
    {
        int pwwValue = lightPotenciometr.analogReqad(); // 2544; //analogRead(LedWorkConfig::PWWPOTENCIOMETR_PIN);

        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 90);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
        vTaskDelay(pdMS_TO_TICKS(1000));

        for (int i = 50; i <= 150; i += 1)
        {
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, i);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(10));
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
        for (int i = 150; i >= 50; i -= 1)
        {
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, i);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
        // printf("pww: %d", pwwValue); //vTaskDelay(1000);
        // vTaskDelay(1000);
        // printf("main thread %d \n", (++i));
    }
}
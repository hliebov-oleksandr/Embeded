#include "driver/mcpwm.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define SERVO_PIN 6

void app_main(void) {
    // Настройка MCPWM
    mcpwm_gpio_init(MCPWM_UNIT_0, MCPWM0A, SERVO_PIN);

    mcpwm_config_t pwm_config;
    pwm_config.frequency = 50;    // 50 Гц для серво
    pwm_config.cmpr_a = 0;        // duty cycle A
    pwm_config.cmpr_b = 0;        // duty cycle B
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config);

    while (1) {
        // Вращение влево (1000 мкс)
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1000);
        vTaskDelay(pdMS_TO_TICKS(2000));

        // Стоп (1500 мкс)
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 1500);
        vTaskDelay(pdMS_TO_TICKS(2000));

        // Вращение вправо (2000 мкс)
        mcpwm_set_duty_in_us(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, 2000);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
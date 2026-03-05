#include "stdio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"

void readLightingTraficSensorTask(void* params) {
    while (true)
    {
        vTaskDelay(1000);
        printf("sensor thread \n");
    }    
}

void startTraficLight(void *params) {
    while (true)
    {
        vTaskDelay(1000);
        printf("light trafic thread \n");   
    }   
}

void app_main() {
    
    //TaskHandle_t *potenciometTaskHandler;    
    printf("Hello world ");
    xTaskCreate(readLightingTraficSensorTask, "readLightingTraficSensorTask", 2048, NULL, 1, NULL);
    xTaskCreate(startTraficLight, "startTraficLight", 2048, NULL, 1, NULL);
    
    int i = 0;
    while (true)
    {
        vTaskDelay(1000);
        printf("main thread %d \n", (++i));
    }    
}
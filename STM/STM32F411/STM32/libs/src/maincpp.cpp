#include "maincpp.h"
#include "stmgpio.h"
#include "string.h"
#include "fonts.h"
#include "ssd1306.h"
#include "bme280.h"
#include "stdio.h"

//extern "C" s32 bme280_init();
extern "C" s32 bme280_data_readout_template(void);
extern "C" s32 bme280_data_readout_template(void);
extern "C" BME280_RETURN_FUNCTION_TYPE bme280_read_pressure_temperature_humidity(
u32 *v_pressure_u32, s32 *v_temperature_s32, u32 *v_humidity_u32);

typedef struct {
    s32 temp;
    u32 press;
    u32 hum;
} bme280_values_t;

/*
#include "usbd_cdc_if.h"

uint8_t digital120_serial_send(const uint8_t * data, const uint16_t len){
   if (data == NULL) {
       return 0;
   }

   CDC_Transmit_FS(data, len);

   return 1;
}
uint8_t logData[200];
uint16_t logDataLen;
*/


STMGPIO gpioBlueLed(GPIOA, GPIO_PIN_5);
char data[] = "HELLO World AAAA AA\n";

void setup()
{
    SSD1306_Init();
    SSD1306_GotoXY(0, 0);
    
    SSD1306_GotoXY(0, 30);
    SSD1306_Puts("TIME 12:23:00", &Font_11x18, SSD1306_COLOR_t::SSD1306_COLOR_WHITE);
    SSD1306_UpdateScreen();
    HAL_Delay(1000);
    SSD1306_ScrollRight(0, 17);
    HAL_Delay(3000);
    SSD1306_ScrollLeft(0, 17);
    HAL_Delay(3000);
    SSD1306_Stopscroll();
    
    gpioBlueLed.writeGPIO(true);
}

void loop(void *params)
{    
    const char msg[] = "Hello World\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), HAL_MAX_DELAY);
    
    gpioBlueLed.writeGPIO(!gpioBlueLed.readGPIO());
    HAL_Delay(2000);

    
    bme280_values_t termo;
    s32 status = bme280_data_readout_template();
    
    char buf[32];
    int len = snprintf(buf, sizeof(buf), "status=%ld\r\n", (long)status);
    HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, HAL_MAX_DELAY);             
    if (status == 0) {
        bme280_read_pressure_temperature_humidity(&termo.press, &termo.temp, &termo.hum);        
        char buf[64];
        int len = snprintf(buf, sizeof(buf),
                           "T=%ld C, P=%lu Pa, H=%lu %%\r\n",
                           (long)termo.temp/100,
                           (unsigned long)termo.press/100,
                           (unsigned long)termo.hum/1024);
        HAL_UART_Transmit(&huart2, (uint8_t*)buf, len, HAL_MAX_DELAY);
    } else {
        const char err[] = "BME280 read error\r\n";
        HAL_UART_Transmit(&huart2, (uint8_t*)err, strlen(err), HAL_MAX_DELAY);
    }
    
    SSD1306_ScrollRight(0, 17);    
    SSD1306_ScrollLeft(0, 17);
}

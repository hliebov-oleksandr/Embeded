#include "maincpp.h"
#include "stmgpio.h"
#include "string.h"
#include "fonts.h"
#include "ssd1306.h"
#include "bme280.h"
#include "stdio.h"

// extern "C" s32 bme280_init();
extern "C" s32 bme280_data_readout_template(void);
extern "C" s32 bme280_data_readout_template(void);
extern "C" BME280_RETURN_FUNCTION_TYPE bme280_read_pressure_temperature_humidity(
    u32 *v_pressure_u32, s32 *v_temperature_s32, u32 *v_humidity_u32);

typedef struct
{
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
}

void writeData(char *data)
{
    
    int len = strlen(data);
    HAL_UART_Transmit(&huart2, (uint8_t *)data, len, HAL_MAX_DELAY);
    char *delimetr = "\n";
    const char *start = data;
    const char *found_str;
    int position, lasrposition;
    char splitedData[3][64];
    int countRows = 0;

    while ((found_str = strstr(start, delimetr)) != NULL)
    {
        position = found_str - start;        
        memcpy(splitedData[countRows], start, sizeof(char) * position);
        splitedData[countRows][position] = '\0';
        HAL_UART_Transmit(&huart2, (uint8_t *)splitedData[countRows], position, HAL_MAX_DELAY);
        start = found_str + strlen(delimetr);
        countRows++;
    }

    SSD1306_Clear();
    int j = 10;
    for (int i = 0; i < countRows; i++)
    {
        //
        SSD1306_GotoXY(0, j);
        SSD1306_Puts(splitedData[i], &Font_11x18, SSD1306_COLOR_t::SSD1306_COLOR_WHITE);
        SSD1306_UpdateScreen();
        j += 16;        
        //
    }
    
    HAL_UART_Transmit(&huart2, (uint8_t *)data, len, HAL_MAX_DELAY);
}

void loop(void *params)
{
    const char msg[] = "Hello World\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, strlen(msg), HAL_MAX_DELAY);

    gpioBlueLed.writeGPIO(!gpioBlueLed.readGPIO());
    HAL_Delay(2000);

    bme280_values_t termo;
    s32 status = bme280_data_readout_template();

    char buf[32];
    int len = snprintf(buf, sizeof(buf), "status=%ld\r\n", (long)status);
    writeData(buf);
    if (status == 0)
    {
        bme280_read_pressure_temperature_humidity(&termo.press, &termo.temp, &termo.hum);
        char buf[64];
        int len = snprintf(buf, sizeof(buf),
                           "T=%ld C \nP=%lu Pa \nH=%lu %% \n",
                           (long)termo.temp / 100,
                           (unsigned long)termo.press / 100,
                           (unsigned long)termo.hum / 1024);
        writeData(buf);
    }
    else
    {
        char err[] = "bme280 \nread \nerror\n";
        writeData(err);
    }
}

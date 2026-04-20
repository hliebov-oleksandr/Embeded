#include "maincpp.h"
#include "stmgpio.h"
#include "string.h"
#include "fonts.h"
#include "ssd1306.h"
#include "bme280.h"
#include "stdio.h"
#include "utils.h"

// extern "C" s32 bme280_init();
extern "C" TIM_HandleTypeDef htim1;
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

volatile uint32_t ms_counter = 0; // глобальный счётчик миллисекунд

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1) // проверяем, что это наш таймер
    {
        ms_counter++; // увеличиваем счётчик каждую 1 мс
        if (ms_counter % 1000 == 0)
        {
            char buf[32];
            int len = sprintf(buf, "counter: %d \r\n", ms_counter);
            HAL_UART_Transmit(&huart2, (uint8_t *)buf, len, HAL_MAX_DELAY);

            SSD1306_GotoXY(0, 30);
            SSD1306_Puts(buf, &Font_11x18, SSD1306_COLOR_t::SSD1306_COLOR_WHITE);
            SSD1306_UpdateScreen();
        }
    }
}

STMGPIO gpioBlueLed(GPIOA, GPIO_PIN_5);
char data[] = "HELLO World AAAA AA\n";

void setup()
{
    HAL_TIM_Base_Start_IT(&htim1);
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
    
    const int maxRowCount = 4;
    char splitedData[maxRowCount][64];
    char *ptr[maxRowCount];    
    for (int i = 0; i < maxRowCount; i++) ptr[i] = splitedData[i];

    int countRows = Utils::split(data, delimetr, ptr, maxRowCount);
   
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

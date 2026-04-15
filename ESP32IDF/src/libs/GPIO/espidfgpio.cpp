#include "espidfgpio.h"
#include "driver/gpio.h"

EspIDFGPIO::EspIDFGPIO(gpio_num_t gpionum, bool isAnalogGPIO) : gpionum(gpionum)
{
    if (isAnalogGPIO)
    {
        adc_chanel1 = ADC1_CHANNEL_MAX;
        adc_chanel2 = ADC2_CHANNEL_MAX;
        initACP();
    }
}

EspIDFGPIO::EspIDFGPIO(gpio_num_t gpionum, gpio_mode_t mode) : gpionum(gpionum), mode(mode)
{
    gpio_set_direction(gpionum, mode);
}

void EspIDFGPIO::writeGPIO(bool levelHigh)
{
    gpio_set_level(gpionum, levelHigh);
}

int EspIDFGPIO::readGPIO()
{
    int level = gpio_get_level(gpionum);
    return level;
}

int EspIDFGPIO::analogRead()
{
    int raw = 0;
    if (adc_chanel1 != ADC1_CHANNEL_MAX)
    {
        raw = adc1_get_raw(adc_chanel1);
    }
    else if (adc_chanel2 != ADC2_CHANNEL_MAX)
    {
        adc2_get_raw(adc_chanel2, ADC_WIDTH_BIT_12, &raw);
    }
    else
    {
        printf("Error: GPIO%d не поддерживает ADC2 or ADC1 \n", gpionum);
    }

    return raw; // return 2045;  gpio_num_t
}

adc1_channel_t EspIDFGPIO::getAnalogChannelADC1()
{
    switch (gpionum)
    {
    case GPIO_NUM_1:
        return ADC1_CHANNEL_0;
    case GPIO_NUM_2:
        return ADC1_CHANNEL_1;
    case GPIO_NUM_3:
        return ADC1_CHANNEL_2;
    case GPIO_NUM_4:
        return ADC1_CHANNEL_3;
    case GPIO_NUM_5:
        return ADC1_CHANNEL_4;
    case GPIO_NUM_6:
        return ADC1_CHANNEL_5;
    case GPIO_NUM_7:
        return ADC1_CHANNEL_6;
    case GPIO_NUM_8:
        return ADC1_CHANNEL_7;
    case GPIO_NUM_9:
        return ADC1_CHANNEL_8;
    case GPIO_NUM_10:
        return ADC1_CHANNEL_9;
    default:
        printf("Error: GPIO%d не поддерживает ADC1 (ESP32-S3)\n", gpionum);
        return ADC1_CHANNEL_MAX;
    }
}

adc2_channel_t EspIDFGPIO::getAnalogChannelADC2()
{
    switch (gpionum)
    {
    case GPIO_NUM_11:
        return ADC2_CHANNEL_0;
    case GPIO_NUM_12:
        return ADC2_CHANNEL_1;
    case GPIO_NUM_13:
        return ADC2_CHANNEL_2;
    case GPIO_NUM_14:
        return ADC2_CHANNEL_3;
    case GPIO_NUM_15:
        return ADC2_CHANNEL_4;
    case GPIO_NUM_16:
        return ADC2_CHANNEL_5;
    case GPIO_NUM_17:
        return ADC2_CHANNEL_6;
    case GPIO_NUM_18:
        return ADC2_CHANNEL_7;
    case GPIO_NUM_19:
        return ADC2_CHANNEL_8;
    case GPIO_NUM_20:
        return ADC2_CHANNEL_9;
    default:
        printf("Error: GPIO%d не поддерживает ADC2 (ESP32-S3)\n", gpionum);
        return ADC2_CHANNEL_MAX;
    }
}

void EspIDFGPIO::initACP()
{
    adc_chanel1 = getAnalogChannelADC1();
    if (adc_chanel1 != ADC1_CHANNEL_MAX)
    {
        adc1_config_width(ADC_WIDTH_BIT_12);                     // 12 бит → 0…4095
        adc1_config_channel_atten(adc_chanel1, ADC_ATTEN_DB_11); // GPIO34, диапазон до ~3.3 В
        return;
    }
    adc_chanel2 = getAnalogChannelADC2();
    if (adc_chanel2 != ADC2_CHANNEL_MAX)
    {
        adc2_config_channel_atten(adc_chanel2, ADC_ATTEN_DB_11); // GPIO34, диапазон до ~3.3 В
    }
}

void EspIDFGPIO::pullUpDown(PulUPDonw type)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << gpionum),                                                           // выбираем пин
        .mode = GPIO_MODE_INPUT,                                                                     // режим входа
        .pull_up_en = (type == PulUPDonw::PULL_UP) ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,       // включаем подтяжку вверх
        .pull_down_en = (type == PulUPDonw::PULL_UP) ? GPIO_PULLDOWN_DISABLE : GPIO_PULLDOWN_ENABLE, // отключаем подтяжку вниз
        .intr_type = GPIO_INTR_DISABLE                                                               // без прерываний
    };
    gpio_config(&io_conf);
}

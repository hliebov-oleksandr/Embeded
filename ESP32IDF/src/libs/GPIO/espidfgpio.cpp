#include "espidfgpio.h"
#include "driver/gpio.h"

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

#ifndef ARDUINOGPIO_H
#define ARDUINOGPIO_H

#include "gpio.h"  
#include "driver/gpio.h"

class EspIDFGPIO : public IGPIO
{
private:
  gpio_num_t gpionum;
  gpio_mode_t mode;
  
public:
  EspIDFGPIO(gpio_num_t gpionum, gpio_mode_t mode);
  void writeGPIO(bool levelHigh) override;
  virtual int readGPIO() override;
};

#endif
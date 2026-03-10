#ifndef ARDUINOGPIO_H
#define ARDUINOGPIO_H

#include "gpio.h"  
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"


class EspIDFGPIO : public IGPIO
{
private:
  gpio_num_t gpionum;
  gpio_mode_t mode;  
  //adc1_channel_t getAnalogChanel();
  adc1_channel_t adc_chanel1;
  adc2_channel_t adc_chanel2;

  adc1_channel_t getAnalogChannelADC1();
  adc2_channel_t getAnalogChannelADC2();
  void initACP();
  
public:
  EspIDFGPIO(gpio_num_t gpionum, bool isAnalogGPIO);
  EspIDFGPIO(gpio_num_t gpionum, gpio_mode_t mode);
  void writeGPIO(bool levelHigh) override;
  int readGPIO() override;
  int analogReqad() override;  
};

#endif
#ifndef ARDUINOGPIO_H
#define ARDUINOGPIO_H

#include "gpio.h"  

class ArduinoGPIO : public IGPIO
{
private:
  int gpionum;

public:
  ArduinoGPIO(int gpionum, int mode);
  void writeGPIO(bool levelHigh) override;
  virtual int readGPIO() override;
  virtual int analogReqad() override;
};

#endif
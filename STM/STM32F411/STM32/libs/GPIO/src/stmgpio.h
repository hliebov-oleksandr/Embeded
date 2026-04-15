#ifndef STMGPIO_H
#define STMGPIO_H

#include "gpio.h"  
#include "stm32f4xx_hal.h"

class STMGPIO : public IGPIO
{

private:
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;

public:  
  STMGPIO(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
  void writeGPIO(bool levelHigh) override;
  int readGPIO() override;
  int analogRead() override;  
  void pullUpDown(PulUPDonw type) override;
};


#endif
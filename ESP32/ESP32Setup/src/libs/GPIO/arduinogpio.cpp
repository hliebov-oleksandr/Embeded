#include "arduinogpio.h"
#include <Arduino.h>

ArduinoGPIO::ArduinoGPIO(int gpionum, int mode) : gpionum(gpionum)
{
    pinMode(gpionum, mode);
}

void ArduinoGPIO::writeGPIO(bool levelHigh)
{
    if (levelHigh)
    {
        digitalWrite(gpionum, HIGH);
    }
    else
    {
        digitalWrite(gpionum, LOW);
    }
}

int ArduinoGPIO::readGPIO()
{
    return digitalRead(gpionum);
}


int ArduinoGPIO::analogReqad()
{
    return analogRead(gpionum);
}
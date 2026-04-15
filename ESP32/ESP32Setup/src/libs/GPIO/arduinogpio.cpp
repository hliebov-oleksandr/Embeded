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


int ArduinoGPIO::analogRead()
{     
    return 4096;// analogRead(gpionum);
}

void ArduinoGPIO::pullUpDown(PulUPDonw type)
{
   pinMode(gpionum, type == PulUPDonw::PULL_UP ?  PULLUP : PULLDOWN);    
}
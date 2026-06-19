#ifndef PWW_H
#define PWW_H
#include "gpio.h"
#include "utils.h"

class GPIOPWMControl {
private:
    IGPIO* gpio;
    int workFrequencyGZ;
    MicrosecProvider microsecFunc;
    double pwwPeriodInMilisec;
    double duteLightTimeInMilisec;
    long duteLastTimeMicros;
    
    
    //Func dutyFunc;
    //Func offFunc;

public:
    GPIOPWMControl();
    GPIOPWMControl(IGPIO* gpio, int pwwFrequencyGZ, MicrosecProvider microseconds);
    //void setPulseFunc(Func dutyFunc, Func offFunc);
    void setPWWDuteTime(int analogResolutionRange, int currentResolution);
    void pulse();
    IGPIO* getGPIO();
};

#endif // PWW_H
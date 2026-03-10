#ifndef PWW_H
#define PWW_H
#include "gpio.h"

typedef unsigned long (*GetMicrosecFunc)(void);
typedef void (*Func)(void);

class GPIOPWMControl {
private:
    IGPIO* gpio;
    int workFrequencyGZ;
    GetMicrosecFunc microsecFunc;
    double pwwPeriodInMilisec;
    double duteLightTimeInMilisec;
    long duteLastTimeMicros;
    
    
    //Func dutyFunc;
    //Func offFunc;

public:
    GPIOPWMControl();
    GPIOPWMControl(IGPIO* gpio, int pwwFrequencyGZ, GetMicrosecFunc microseconds);
    //void setPulseFunc(Func dutyFunc, Func offFunc);
    void setPWWDuteTime(int analogResolutionRange, int currentResolution);
    void pulse();
    IGPIO* getGPIO();
};

#endif // PWW_H
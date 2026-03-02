#ifndef PWW_H
#define PWW_H

typedef unsigned long (*GetMicrosecFunc)(void);
typedef void (*Func)(void);

class PWMControl {
private:
    int workFrequencyGZ;
    double pwwPeriodInMilisec;
    double duteLightTimeInMilisec;
    long duteLastTimeMicros;
    GetMicrosecFunc microsecFunc;
    Func dutyFunc;
    Func offFunc;

public:
    PWMControl();
    PWMControl(int pwwFrequencyGZ, GetMicrosecFunc microseconds);

    void setPulseFunc(Func dutyFunc, Func offFunc);
    void setPWWDuteTime(int analogResolutionRange, int currentResolution);
    void pulse();
};

#endif // PWW_H
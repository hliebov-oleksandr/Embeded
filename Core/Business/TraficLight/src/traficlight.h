#ifndef TRAFIC_LIGHT
#define TRAFIC_LIGHT
#include "led.h"

typedef enum
{
    GREEN_LIGHT = 0,
    GREEN_BLINK_LIGHT = 1,
    YELLOW_LIGHT = 2,
    RED_LIGHT = 3,
    RED_YELOW_LIGHT = 4
} TrafficLightMode;

// WorkConfig.h
struct TraficWorkConfig
{
public:
    int GREEN_LIGHT_TIME_MILISEC;
    int GREEN_BLINK_LIGHT_TIME_MILISEC;
    int YELLOW_LIGHT_TIME_MILISEC;
    int RED_LIGHT_MILISEC;
    int RED_YELLOW_LIGHT_MILISEC;    
};

class TraficLightController
{

private:
    Led *ledGreen;
    Led *ledYellow;
    Led *ledRed;
    TraficWorkConfig traficConfig;
    int TRAFIC_PHASE_DURATIONS[5];
    
public:
    TraficLightController(Led *ledGreen, Led *ledYellow, Led *ledRed, const TraficWorkConfig traficConfig);
    void LightTrafic(long work_period_count_milisec);
    int getTraficAllWorkPeriod();
    TrafficLightMode getCurrentMode(long work_period_count_milisec);
};

#endif
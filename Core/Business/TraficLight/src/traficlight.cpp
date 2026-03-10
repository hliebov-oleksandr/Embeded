#include "traficlight.h"

// WorkConfig.cpp

TraficLightController::TraficLightController(Led* ledGreen,Led* ledYellow, Led* ledRed, TraficWorkConfig traficConfig) :
    ledGreen(ledGreen), ledYellow(ledYellow), ledRed(ledRed), traficConfig(traficConfig)
{            
    TRAFIC_PHASE_DURATIONS[TrafficLightMode::GREEN_LIGHT] = traficConfig.GREEN_LIGHT_TIME_MILISEC;
    TRAFIC_PHASE_DURATIONS[GREEN_BLINK_LIGHT] = traficConfig.GREEN_BLINK_LIGHT_TIME_MILISEC;
    TRAFIC_PHASE_DURATIONS[YELLOW_LIGHT] = traficConfig.YELLOW_LIGHT_TIME_MILISEC;
    TRAFIC_PHASE_DURATIONS[RED_LIGHT] = traficConfig.RED_LIGHT_MILISEC;
    TRAFIC_PHASE_DURATIONS[RED_YELOW_LIGHT] = traficConfig.RED_YELLOW_LIGHT_MILISEC;
}

TrafficLightMode TraficLightController::getCurrentMode(long work_period_count_milisec) {
  int duration = 0;
  for (int period = 0; period <= sizeof(TRAFIC_PHASE_DURATIONS)/sizeof(TRAFIC_PHASE_DURATIONS[0]); ++period)
  {
    duration += TRAFIC_PHASE_DURATIONS[period];
    if (work_period_count_milisec < duration)
    {            
      return static_cast<TrafficLightMode>(period);
    }
  }
  return GREEN_LIGHT;
}

int TraficLightController::getTraficAllWorkPeriod() {    
    return traficConfig.GREEN_LIGHT_TIME_MILISEC + traficConfig.GREEN_BLINK_LIGHT_TIME_MILISEC + 
            traficConfig.YELLOW_LIGHT_TIME_MILISEC + traficConfig.RED_LIGHT_MILISEC + 
            traficConfig.RED_YELLOW_LIGHT_MILISEC;
}

void TraficLightController::LightTrafic(long work_period_count_milisec)
{
  long current_mode = getCurrentMode(work_period_count_milisec);
  switch (current_mode)
  {
  case TrafficLightMode::GREEN_LIGHT:
    ledRed->updateState(LedState::OFF);
    ledYellow->updateState(LedState::OFF);
    ledGreen->updateState(LedState::ON);
    break;
  case TrafficLightMode::GREEN_BLINK_LIGHT:
  {
    long currentPhaseMilisec = work_period_count_milisec - traficConfig.GREEN_LIGHT_TIME_MILISEC;
    // алгоритм мигания: 4 раза за фазу
    int blinkCount = 4;
    int blinkPeriod = traficConfig.GREEN_BLINK_LIGHT_TIME_MILISEC / blinkCount;

    if ((currentPhaseMilisec % blinkPeriod) < (blinkPeriod / 2))
    {
      ledGreen->updateState(LedState::OFF);
    }
    else
    {
      ledGreen->updateState(LedState::ON);
    }
  }
  break;
  case TrafficLightMode::YELLOW_LIGHT:
    ledGreen->updateState(LedState::OFF);
    ledYellow->updateState(LedState::ON);
    break;
  case TrafficLightMode::RED_LIGHT:
    ledYellow->updateState(LedState::OFF);
    ledRed->updateState(LedState::ON);
    break;
  case TrafficLightMode::RED_YELOW_LIGHT:
    ledRed->updateState(LedState::ON);
    ledYellow->updateState(LedState::ON);
    break;
  default:
    break;
  }
}
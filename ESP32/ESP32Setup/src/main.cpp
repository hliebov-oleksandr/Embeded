#include <Arduino.h>
#include "gpiopwm.h"
#include "gpio.h"
#include "led.h"
#include "libs/GPIO/arduinogpio.h"

int getMode();

typedef enum
{
  GREEN_LIGHT = 0,
  GREEN_BLINK_LIGHT = 1,
  YELLOW_LIGHT = 2,
  RED_LIGHT = 3,
  RED_YELOW_LIGHT = 4
} TrafficLightMode;

class LedWorkConfig
{
public:
  static constexpr int LED_PIN_GREEN = 10;
  static constexpr int LED_PIN_YELLOW = 13;
  static constexpr int LED_PIN_RED = 11;
  static constexpr int PWWPOTENCIOMETR_PIN = 12;

  static constexpr int PWW_LED_FREQUENCY_GZ = 1000;
  static constexpr int ANALOG_RESOLUTION = 4095;

  static constexpr int GREEN_LIGHT_TIME_MILISEC = 6000;
  static constexpr int GREEN_BLINK_LIGHT_TIME_MILISEC = 2000;
  static constexpr int YELLOW_LIGHT_TIME_MILISEC = 2000;
  static constexpr int RED_LIGHT_MILISEC = GREEN_LIGHT_TIME_MILISEC;
  static constexpr int RRED_YELOW_LIGHT_MILISEC = YELLOW_LIGHT_TIME_MILISEC;
  static constexpr int TRAFIC_ALL_LIGHT_WORK_PERIOD_MILISEC = GREEN_LIGHT_TIME_MILISEC +
                                                              GREEN_BLINK_LIGHT_TIME_MILISEC + YELLOW_LIGHT_TIME_MILISEC +
                                                              RED_LIGHT_MILISEC + RRED_YELOW_LIGHT_MILISEC;
};

static constexpr int PhaseDurations[5] = {
    [GREEN_LIGHT] = LedWorkConfig::GREEN_LIGHT_TIME_MILISEC,
    [GREEN_BLINK_LIGHT] = LedWorkConfig::GREEN_BLINK_LIGHT_TIME_MILISEC,
    [YELLOW_LIGHT] = LedWorkConfig::YELLOW_LIGHT_TIME_MILISEC,
    [RED_LIGHT] = LedWorkConfig::RED_LIGHT_MILISEC,
    [RED_YELOW_LIGHT] = LedWorkConfig::RRED_YELOW_LIGHT_MILISEC};

ArduinoGPIO greenGPIO(LedWorkConfig::LED_PIN_GREEN, OUTPUT);
ArduinoGPIO yellowGPIO(LedWorkConfig::LED_PIN_YELLOW, OUTPUT);
ArduinoGPIO redGPIO(LedWorkConfig::LED_PIN_RED, OUTPUT);

GPIOPWMControl pwwControlGreen(&greenGPIO, LedWorkConfig::PWW_LED_FREQUENCY_GZ, micros);
GPIOPWMControl pwwControlYellow(&yellowGPIO, LedWorkConfig::PWW_LED_FREQUENCY_GZ, micros);
GPIOPWMControl pwwControlRed(&redGPIO, LedWorkConfig::PWW_LED_FREQUENCY_GZ, micros);

Led ledGreen(&pwwControlGreen);
Led ledYellow(&pwwControlYellow);
Led ledRed(&pwwControlRed);

volatile long period_count_milisec = 0;

void IRAM_ATTR onTimer()
{
  period_count_milisec++;
  if (period_count_milisec >= LedWorkConfig::TRAFIC_ALL_LIGHT_WORK_PERIOD_MILISEC)
  {
    period_count_milisec = 0;
  }
}

void setup()
{
  analogSetAttenuation(ADC_11db);
  hw_timer_t *timer = timerBegin(0, 80, true); // таймер на ESP32
  timerAttachInterrupt(timer, onTimer, true);
  timerAlarmWrite(timer, 1000, true); // кожну mil sec секунду 000
  timerAlarmEnable(timer);
  Serial.begin(115200);
  Serial.println("HELLO World");
}

int getMode()
{
  int duration = 0;
  for (int period = 0; period <= RED_YELOW_LIGHT; ++period)
  {
    duration += PhaseDurations[period];
    if (period_count_milisec < duration)
    {
      return period;
    }
  }
  return GREEN_LIGHT;
}

void loop()
{
  int pwwValue = analogRead(LedWorkConfig::PWWPOTENCIOMETR_PIN);
  pwwControlGreen.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
  pwwControlYellow.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
  pwwControlRed.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);

  long current_mode = getMode();

  switch (current_mode)
  {
  case TrafficLightMode::GREEN_LIGHT:
    ledRed.updateState(LedState::OFF);
    ledYellow.updateState(LedState::OFF);
    ledGreen.updateState(LedState::ON);
    break;
  case TrafficLightMode::GREEN_BLINK_LIGHT:
  {
    long currentPhaseMilisec = period_count_milisec - LedWorkConfig::GREEN_LIGHT_TIME_MILISEC;
    // алгоритм мигания: 4 раза за фазу
    int blinkCount = 4;
    int blinkPeriod = LedWorkConfig::GREEN_BLINK_LIGHT_TIME_MILISEC / blinkCount;

    if ((currentPhaseMilisec % blinkPeriod) < (blinkPeriod / 2))
    {
      ledGreen.updateState(LedState::OFF);
    }
    else
    {
      ledGreen.updateState(LedState::ON);
    }
  }
  break;
  case TrafficLightMode::YELLOW_LIGHT:
    ledGreen.updateState(LedState::OFF);
    ledYellow.updateState(LedState::ON);
    break;
  case TrafficLightMode::RED_LIGHT:
    ledYellow.updateState(LedState::OFF);
    ledRed.updateState(LedState::ON);
    break;
  case TrafficLightMode::RED_YELOW_LIGHT:
    ledRed.updateState(LedState::ON);
    ledYellow.updateState(LedState::ON);
    break;
  default:
    break;
  }
}
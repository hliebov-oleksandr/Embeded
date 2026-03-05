#include <Arduino.h>
#include "gpiopwm.h"
#include "gpio.h"
#include "led.h"
#include "libs/GPIO/arduinogpio.h"
#include "traficlight.h"


class LedWorkConfig
{
public:
  static constexpr int LED_PIN_GREEN = 10;
  static constexpr int LED_PIN_YELLOW = 13;
  static constexpr int LED_PIN_RED = 11;
  static constexpr int PWWPOTENCIOMETR_PIN = 12;

  static constexpr int PWW_LED_FREQUENCY_GZ = 1000;
  static constexpr int ANALOG_RESOLUTION = 4095;
};

ArduinoGPIO greenGPIO(LedWorkConfig::LED_PIN_GREEN, OUTPUT);
ArduinoGPIO yellowGPIO(LedWorkConfig::LED_PIN_YELLOW, OUTPUT);
ArduinoGPIO redGPIO(LedWorkConfig::LED_PIN_RED, OUTPUT);

GPIOPWMControl pwwControlGreen(&greenGPIO, LedWorkConfig::PWW_LED_FREQUENCY_GZ, micros);
GPIOPWMControl pwwControlYellow(&yellowGPIO, LedWorkConfig::PWW_LED_FREQUENCY_GZ, micros);
GPIOPWMControl pwwControlRed(&redGPIO, LedWorkConfig::PWW_LED_FREQUENCY_GZ, micros);

Led ledGreen(&pwwControlGreen);
Led ledYellow(&pwwControlYellow);
Led ledRed(&pwwControlRed);

TraficWorkConfig traficConfig = {
    .GREEN_LIGHT_TIME_MILISEC = 6000,
    .GREEN_BLINK_LIGHT_TIME_MILISEC = 2000,
    .YELLOW_LIGHT_TIME_MILISEC = 2000,
    .RED_LIGHT_MILISEC = 6000,
    .RED_YELLOW_LIGHT_MILISEC = 2000 };

TraficLightController traficController(&ledGreen, &ledYellow, &ledRed, traficConfig);
volatile long period_count_milisec = 0;

void IRAM_ATTR onTimer()
{
  period_count_milisec++;
  if (period_count_milisec >= traficController.getTraficAllWorkPeriod())
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

void loop()
{
  int pwwValue = analogRead(LedWorkConfig::PWWPOTENCIOMETR_PIN);

  pwwControlGreen.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
  pwwControlYellow.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);
  pwwControlRed.setPWWDuteTime(LedWorkConfig::ANALOG_RESOLUTION, pwwValue);

  traficController.LightTrafic(period_count_milisec);
}
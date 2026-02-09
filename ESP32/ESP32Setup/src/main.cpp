#include <Arduino.h>

bool isButtonPressed(int pin);
void pinPulse(int pin, int pulseDudeTime, int pulseLowTime);
void pinBlinkInPWWPulse(int pin, int pulseTime, float pwwDuteTime, float pwwPeriod);
void startBlinking();
void readAnalogSensors();


#define CONTROL_ANALOG_PHOTO_PIN_1 1
#define CONTROL_ANALOG_POTENCIOMETR_PIN_2 2
#define CONTROL_LED_PIN_1 10
#define CONTROL_LED_PIN_2 11
#define CONTROL_BUTTON_PIN 9
#define FREQUENCY_PWW_GZ 1000 // 1 Kgz
const float PWW_PERIOD_TIME_MILISEC = (1 / double(FREQUENCY_PWW_GZ)) * 1000;
float pwwDuteTime = PWW_PERIOD_TIME_MILISEC;
//esp 32 s3 series

void setup() {
  pinMode(CONTROL_LED_PIN_1 , OUTPUT);
  pinMode(CONTROL_LED_PIN_2 , OUTPUT);
  pinMode(CONTROL_BUTTON_PIN , INPUT_PULLDOWN);
  analogSetAttenuation(ADC_11db);
  Serial.begin(115200);  
}

void loop() {  
  if (!isButtonPressed(CONTROL_BUTTON_PIN)) {
    readAnalogSensors();    
    pinBlinkInPWWPulse(CONTROL_LED_PIN_1, 500, pwwDuteTime, PWW_PERIOD_TIME_MILISEC);
    pinBlinkInPWWPulse(CONTROL_LED_PIN_2, 500, pwwDuteTime, PWW_PERIOD_TIME_MILISEC);
    //startBlinking();      
  }   
}

bool isButtonPressed(int pin) {
  return digitalRead(pin) == HIGH ? delay(10), digitalRead(pin) == HIGH : false;
}

void startBlinking() {
  pinPulse(CONTROL_LED_PIN_1, 1000, 0);
  pinPulse(CONTROL_LED_PIN_2, 1000, 0);  
  // Stm 32 F1411ccux
}
// put function definitions here

void pinBlinkInPWWPulse(int pin, int pulseTime, float pwwDuteTime, float pwwPeriod) {
  long endTime = millis() + pulseTime;
  while (millis() <= endTime) {
      if (pwwDuteTime > 0){
        digitalWrite(pin, HIGH);          
      }      
      delay(pwwDuteTime);
      digitalWrite(pin, LOW);  
      delay(pwwPeriod - pwwDuteTime);
  }
  digitalWrite(pin, LOW);
  delay(pulseTime);
}

void pinPulse(int pin, int pulseDudeTime, int pulseLowTime) {
  digitalWrite(pin, HIGH);
  delay(pulseDudeTime);
  digitalWrite(pin, LOW);
  delay(pulseLowTime);
}

void readAnalogSensors() {
  int analogPhotoResistorVal = analogRead(CONTROL_ANALOG_PHOTO_PIN_1);  
  int analogPotenciometrPWWVal = analogRead(CONTROL_ANALOG_POTENCIOMETR_PIN_2);  
  pwwDuteTime = (double(analogPotenciometrPWWVal) / 4095.0) * PWW_PERIOD_TIME_MILISEC;

  Serial.print("Potenciometr val ");
  Serial.println(analogPotenciometrPWWVal);
  Serial.print("Milsec Dute Time ");
  Serial.println(pwwDuteTime);
  
  Serial.print("Photo Resistor ");
  Serial.println(analogPhotoResistorVal);  
  Serial.println((analogPhotoResistorVal / 4095.0) * 3.3);  
}


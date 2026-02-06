#include <Arduino.h>

bool isButtonPressed(int pin);
void pinPulse(int pin, int pulseDudeTime, int pulseLowTime);
void startBlinking();
void readPhotoSensor();

#define CONTROL_PHOTO_PIN_1 1
#define CONTROL_LED_PIN_1 10
#define CONTROL_LED_PIN_2 11
#define CONTROL_BUTTON_PIN 9
//esp 32 s3 series

void setup() {
  pinMode(CONTROL_LED_PIN_1 , OUTPUT);
  pinMode(CONTROL_LED_PIN_2 , OUTPUT);
  pinMode(CONTROL_BUTTON_PIN , INPUT_PULLDOWN);
  analogSetAttenuation(ADC_11db);
  Serial.begin(115200);  
}

void loop() {  
  if (isButtonPressed(CONTROL_BUTTON_PIN)) {
    readPhotoSensor();
    startBlinking();      
  }   
}

bool isButtonPressed(int pin) {
  return digitalRead(pin) == HIGH ? delay(10), digitalRead(pin) == HIGH : false;
}

void startBlinking() {

  Serial.println("LED on");  
  pinPulse(CONTROL_LED_PIN_1, 500, 0);
  pinPulse(CONTROL_LED_PIN_2, 500, 0);
  Serial.println("LED off");
  // Stm 32 F1411ccux
}
// put function definitions here

void pinPulse(int pin, int pulseDudeTime, int pulseLowTime) {
  digitalWrite(pin, HIGH);
  delay(pulseDudeTime);
  digitalWrite(pin, LOW);
  delay(pulseLowTime);
}

void readPhotoSensor() {
  int analogVal = analogRead(CONTROL_PHOTO_PIN_1);
  Serial.println(analogVal);  
  Serial.println((analogVal / 4095.0) * 3.3);  
}
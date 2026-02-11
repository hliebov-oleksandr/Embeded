#include <Arduino.h>


#define CONTROL_ANALOG_PHOTO_PIN_1 1
#define CONTROL_ANALOG_POTENCIOMETR_PIN_2 2
#define CONTROL_LED_PIN_1 10
#define CONTROL_LED_PIN_2 11
#define CONTROL_BUTTON_PIN 9

typedef struct {    
   int PWWFrequencyGZ;  
   int* PWWPins;   
   int PWWPinsLength;   
   float PWWPeriodMilisec;
   float PWWDuteTimeMilisec;   
} PWMConfig;
int ledPins[] = { CONTROL_LED_PIN_1, CONTROL_LED_PIN_2 };
int ledPin1[] = { CONTROL_LED_PIN_1 };
int ledPin2[] = { CONTROL_LED_PIN_2 };
PWMConfig ledPWWCofig = { .PWWFrequencyGZ = 1000,
                          .PWWPins = ledPins,
                          .PWWPinsLength = 2,
                          .PWWPeriodMilisec = (1.0f / 1000) * 1000.0f }; // (1 sec /  PWWFrequencyGZ) * 1000 ForMilisec                          
PWMConfig ledPWWCofig1Led = ledPWWCofig;
PWMConfig ledPWWCofig2Led = ledPWWCofig;

bool isButtonPressed(int pin);
void pinPulse(int pin, int pulseDudeTime, int pulseLowTime);
void pinBlinkInPWWPulse(const PWMConfig& pwwCnfg, int pulseTime);
void pinLightInPWWPulse(const PWMConfig& pwwCnfg, int pulseTime);
void startBlinking();
void readAnalogSensors();

//esp 32 s3 series

void setup() {
  pinMode(CONTROL_LED_PIN_1 , OUTPUT);
  pinMode(CONTROL_LED_PIN_2 , OUTPUT);
  pinMode(CONTROL_BUTTON_PIN , INPUT_PULLDOWN);
  analogSetAttenuation(ADC_11db);
  Serial.begin(115200);  
  ledPWWCofig1Led.PWWDuteTimeMilisec = ledPWWCofig2Led.PWWDuteTimeMilisec = ledPWWCofig.PWWDuteTimeMilisec = ledPWWCofig.PWWPeriodMilisec;                          
  ledPWWCofig1Led.PWWPins = ledPin1;
  ledPWWCofig2Led.PWWPins = ledPin2;
  ledPWWCofig1Led.PWWPinsLength = ledPWWCofig2Led.PWWPinsLength = 1;
}

void loop() {    
  
  readAnalogSensors();    
  if (!isButtonPressed(CONTROL_BUTTON_PIN)) { // Start Blinking
    pinBlinkInPWWPulse(ledPWWCofig1Led, 500);
    pinBlinkInPWWPulse(ledPWWCofig2Led, 500);    
  } else { // Lighting both leds       
    pinLightInPWWPulse(ledPWWCofig, 1000);    
  }
}

bool isButtonPressed(int pin) {
  return digitalRead(pin) == HIGH ? delay(5), digitalRead(pin) == HIGH : false;
}

void startBlinking() {
  pinPulse(CONTROL_LED_PIN_1, 1000, 0);
  pinPulse(CONTROL_LED_PIN_2, 1000, 0);  
  // Stm 32 F1411ccux
}
// put function definitions here

void pinLightInPWWPulse(const PWMConfig& pwwCnfg, int pulseTime) {
  long endTime = millis() + pwwCnfg.PWWPeriodMilisec;  
  while (millis() <= endTime) {
      if (pwwCnfg.PWWDuteTimeMilisec > 0) {
        for (size_t i = 0; i < pwwCnfg.PWWPinsLength ; i++) {     
          digitalWrite(pwwCnfg.PWWPins[i], HIGH);        
        }              
      }      
      delay(pwwCnfg.PWWDuteTimeMilisec);      
      for (size_t i = 0; i <  pwwCnfg.PWWPinsLength ; i++) {     
          digitalWrite(pwwCnfg.PWWPins[i], LOW);              
      }                    
      delay(pwwCnfg.PWWPeriodMilisec - pwwCnfg.PWWDuteTimeMilisec);
  }  
}

void pinBlinkInPWWPulse(const PWMConfig& pwwCnfg, int pulseTime) {  
  pinLightInPWWPulse(pwwCnfg, pulseTime);
  for (size_t i = 0; i < pwwCnfg.PWWPinsLength; i++) {    
    digitalWrite(pwwCnfg.PWWPins[i], LOW);
   }  
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
  
  ledPWWCofig1Led.PWWDuteTimeMilisec = ledPWWCofig2Led.PWWDuteTimeMilisec = 
  ledPWWCofig.PWWDuteTimeMilisec  = (double(analogPotenciometrPWWVal) / 4095.0) * ledPWWCofig.PWWPeriodMilisec;

  Serial.print("Potenciometr val ");
  Serial.println(analogPotenciometrPWWVal);
  Serial.print("Milsec Dute Time ");
  Serial.println(ledPWWCofig.PWWDuteTimeMilisec);

  Serial.print("Photo Resistor ");
  Serial.println(analogPhotoResistorVal);  
  Serial.println((analogPhotoResistorVal / 4095.0) * 3.3);  
}


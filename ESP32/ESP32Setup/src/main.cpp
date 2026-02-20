#include <Arduino.h>

void buttonLedChangeModeClick();

enum class LedState {
  ON, OFF
};

class LedWorkGonfig {
  public:
      static constexpr int LED_PIN_1 = 10;
      static constexpr int LED_PIN_2 = 11;
      static constexpr int BUTTON_CHANHE_MODE_PIN = 9;
      static constexpr int IS_BUTTON_PRESSED_MILISEC_FLAG = 20;
      static constexpr int PERIOD_TO_CHANGE_LIGHT_MODE_MILISEC = 1000;
      static constexpr int PWWPOTENCIOMETR_PIN = 12;      
      static constexpr int ANALOG_RESOLUTION = 4095;      
};

class PWWLed {  
  private:
      int ledPin;
      LedState state;      

  public:

      PWWLed(int pin) : ledPin(pin), state(LedState::OFF) {              
        pinMode(ledPin, OUTPUT);
      }           

      void updateState(LedState state) {
          this->state = state;
          if (state == LedState::ON) {
            digitalWrite(this->ledPin, HIGH);
          } else {
            digitalWrite(this->ledPin, LOW);
          }
      }

      LedState getState(){
          return this->state;
      }
};

PWWLed led1(LedWorkGonfig::LED_PIN_1);
PWWLed led2(LedWorkGonfig::LED_PIN_2);

volatile bool isButtonPressed = false;

void setup() {
  analogSetAttenuation(ADC_11db);  
  Serial.begin(115200);    
  pinMode(LedWorkGonfig::BUTTON_CHANHE_MODE_PIN, INPUT_PULLDOWN);
  attachInterrupt(LedWorkGonfig::BUTTON_CHANHE_MODE_PIN, buttonLedChangeModeClick, CHANGE);  
}

void loop() {        
  static long workTime = 0;

  if (!isButtonPressed) {
    led1.updateState(LedState::ON);
    led2.updateState(LedState::ON);
    workTime = 0;
  } else {
      
      long currentTime = millis();
      long elapsedTime = currentTime - workTime;

      //mode 1 Blink Light
      if (elapsedTime <= LedWorkGonfig::PERIOD_TO_CHANGE_LIGHT_MODE_MILISEC) {
        bool isOn = (currentTime / 50) % 2 == 0;
        if (isOn) {
          led1.updateState(LedState::ON);
          led2.updateState(LedState::ON);
        } else {
          led1.updateState(LedState::OFF);
          led2.updateState(LedState::OFF);
        }
      }
      //mode 2 Turn On
      else if (elapsedTime > LedWorkGonfig::PERIOD_TO_CHANGE_LIGHT_MODE_MILISEC && 
        elapsedTime <= LedWorkGonfig::PERIOD_TO_CHANGE_LIGHT_MODE_MILISEC * 2)
      {
           led1.updateState(LedState::ON);    
           led2.updateState(LedState::ON);
      }
      //mode 3 Turn off
      else if (elapsedTime > LedWorkGonfig::PERIOD_TO_CHANGE_LIGHT_MODE_MILISEC * 2 && 
        elapsedTime <= LedWorkGonfig::PERIOD_TO_CHANGE_LIGHT_MODE_MILISEC * 3)
      {
          led1.updateState(LedState::OFF);    
          led2.updateState(LedState::OFF);
      }
      else {
        workTime = currentTime;
      }
  }
   
}

void IRAM_ATTR buttonLedChangeModeClick() {   
    long currentTime = millis();
    if (digitalRead(LedWorkGonfig::BUTTON_CHANHE_MODE_PIN) == HIGH) {
       isButtonPressed = true;       
    } else if (digitalRead(LedWorkGonfig::BUTTON_CHANHE_MODE_PIN) == LOW){
      isButtonPressed = false;     
    }    
}

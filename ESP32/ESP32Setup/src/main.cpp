#include <Arduino.h>

enum class LedState {
  ON, OFF
};

class LedWorkConfig {
  public:
      static constexpr int LED_PIN_1 = 10;
      static constexpr int LED_PIN_2 = 11;
      static constexpr int LED_PIN_3 = 13;     
};

class Led {  
  private:
      int ledPin;
      LedState state;      
      long lastStateChange;
  public:

      Led(int pin) : ledPin(pin), state(LedState::OFF), lastStateChange(0){              
        pinMode(ledPin, OUTPUT);
      }           

      void changeState() {
         updateState(this->state == LedState::ON ? LedState::OFF : LedState::ON);
         lastStateChange = millis();
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

      long getLastStateChange() {
        return this->lastStateChange;
      }
};

Led led1(LedWorkConfig::LED_PIN_1);
Led led2(LedWorkConfig::LED_PIN_2);
Led led3(LedWorkConfig::LED_PIN_3);

void setup() {  
  Serial.begin(115200);        
}

void loop() {        

    long currentTime = millis();
    
    if (currentTime - led1.getLastStateChange() > 200) {
       led1.changeState();
    }

    if (currentTime - led2.getLastStateChange() > 500) {
       led2.changeState();
    }

    if (currentTime - led3.getLastStateChange() > 1000) {
       led3.changeState();
    }
    
}


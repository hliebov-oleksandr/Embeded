#include <Arduino.h>

enum class LedState {
  ON, OFF
};

class LedWorkGonfig {
  public:
      static constexpr int LED_PIN_1 = 10;
      static constexpr int LED_PIN_2 = 11;
      static constexpr int BUTTON_CHANHE_MODE_PIN = 9;
      static constexpr int PWWPOTENCIOMETR_PIN = 12;
      static constexpr double PWW_LED_FREQUENCY_GZ = 1000.0;      
      static constexpr int ANALOG_RESOLUTION = 4095;      
};

class PWWLed {  
  private:
      int ledPin;
      LedState state;
      int workFrequencyGZ;
      double pwwLedPeriodInMilisec;       
      double duteLightTimeInMilisec;
      long ledDuteLastTimeMicros;

  public:
      PWWLed(int pin, int pwwFrequencyGZ) : ledPin(pin), workFrequencyGZ(pwwFrequencyGZ) , state(LedState::OFF) {      
        this->pwwLedPeriodInMilisec =  (1 / static_cast<double>(pwwFrequencyGZ)) * 1000.0;
        this->duteLightTimeInMilisec = this->pwwLedPeriodInMilisec;
        this->ledDuteLastTimeMicros = 0;
        pinMode(ledPin, OUTPUT);
      }

      void setPWWDuteLightTime(int analogResolutionRange, int currentResolution) {
        //Serial.print("Analog :");
        //Serial.println(currentResolution);
        double duteTimeMilisec = (static_cast<double>(currentResolution) / static_cast<double>(analogResolutionRange)) * this->pwwLedPeriodInMilisec;
        this->duteLightTimeInMilisec =  duteTimeMilisec;
        //Serial.println(duteTimeMilisec);
      }      

      void updateState(LedState state) {
          this->state = state;
          if (state == LedState::ON) {
              unsigned long currentTime = micros(); // точность до микросекунд
              unsigned long elapsed = currentTime - this->ledDuteLastTimeMicros;

              // Переводим миллисекунды в микросекунды для сравнения
              double duteTimeMicros = this->duteLightTimeInMilisec * 1000.0;
              double periodMicros   = this->pwwLedPeriodInMilisec * 1000.0;

              if (elapsed < duteTimeMicros) {
                  digitalWrite(this->ledPin, HIGH);
              } else if (elapsed < periodMicros) {
                  digitalWrite(this->ledPin, LOW);
              } else {
                  this->ledDuteLastTimeMicros = currentTime; // начинаем новый цикл
              }
          } else {
              digitalWrite(this->ledPin, LOW);
          }
      }

      LedState getState(){
          return this->state;
      }
};

PWWLed led1(LedWorkGonfig::LED_PIN_1, LedWorkGonfig::PWW_LED_FREQUENCY_GZ);
PWWLed led2(LedWorkGonfig::LED_PIN_2, LedWorkGonfig::PWW_LED_FREQUENCY_GZ);


void setup() {
  analogSetAttenuation(ADC_11db);
  Serial.begin(115200);  
}

void loop() {    
    int pwwValue = analogRead(LedWorkGonfig::PWWPOTENCIOMETR_PIN);
    led1.setPWWDuteLightTime(LedWorkGonfig::ANALOG_RESOLUTION, pwwValue);
    led2.setPWWDuteLightTime(LedWorkGonfig::ANALOG_RESOLUTION, pwwValue);

    led1.updateState(LedState::ON);
    led2.updateState(LedState::ON);
}

bool isButtonPressed(int pin) {
  return digitalRead(pin) == HIGH ? delay(5), digitalRead(pin) == HIGH : false;
}

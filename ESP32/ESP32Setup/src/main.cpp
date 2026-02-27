#include <Arduino.h>
#include "libs/pww.cpp"

int getMode();

// Прототипы функций для зелёного светодиода
void greenOn();
void greenOff();

// Прототипы функций для жёлтого светодиода
void yellowOn();
void yellowOff();

// Прототипы функций для красного светодиода
void redOn();
void redOff();

  enum class LedState {
    ON, OFF
  };

  typedef enum {
    GREEN_LIGHT = 0,
    GREEN_BLINK_LIGHT = 1,
    YELLOW_LIGHT = 2,
    RED_LIGHT = 3,
    RED_YELOW_LIGHT = 4
  } TrafficLightMode;

  class LedWorkConfig {
    public:
        static constexpr int LED_PIN_GREEN = 10;
        static constexpr int LED_PIN_YELLOW = 13;
        static constexpr int LED_PIN_RED = 11;     
        static constexpr int PWWPOTENCIOMETR_PIN = 12;

        static constexpr int PWW_LED_FREQUENCY_GZ = 1000;
        static constexpr int ANALOG_RESOLUTION = 4095; 
        

        static constexpr int GREEN_LIGHT_TIME_MILISEC = 4000;
        static constexpr int GREEN_BLINK_LIGHT_TIME_MILISEC = 1000;
        static constexpr int YELLOW_LIGHT_TIME_MILISEC = 1000;
        static constexpr int RED_LIGHT_MILISEC = GREEN_LIGHT_TIME_MILISEC;
        static constexpr int RRED_YELOW_LIGHT_MILISEC = YELLOW_LIGHT_TIME_MILISEC;
        static constexpr int TRAFIC_ALL_LIGHT_WORK_PERIOD_MILISEC = GREEN_LIGHT_TIME_MILISEC + 
                                  GREEN_BLINK_LIGHT_TIME_MILISEC +  YELLOW_LIGHT_TIME_MILISEC +
                                  RED_LIGHT_MILISEC + RRED_YELOW_LIGHT_MILISEC;                                      
  };

  static constexpr int PhaseDurations[5] =  {
              [GREEN_LIGHT]      = LedWorkConfig::GREEN_LIGHT_TIME_MILISEC,
              [GREEN_BLINK_LIGHT]= LedWorkConfig::GREEN_BLINK_LIGHT_TIME_MILISEC,
              [YELLOW_LIGHT]     = LedWorkConfig::YELLOW_LIGHT_TIME_MILISEC,
              [RED_LIGHT]        = LedWorkConfig::RED_LIGHT_MILISEC,
              [RED_YELOW_LIGHT]  = LedWorkConfig::RRED_YELOW_LIGHT_MILISEC
        };

  class Led {  
    private:
        int ledPin;
        LedState state;     
        PWMControl* pwwControl;

    public:

        Led(int pin) : ledPin(pin), state(LedState::OFF), pwwControl(nullptr){              
          pinMode(ledPin, OUTPUT);
        }           

        Led(int pin, PWMControl* pwwControl) : Led(pin) {              
            this->pwwControl = pwwControl;
        }

        void changeState() {
          updateState(this->state == LedState::ON ? LedState::OFF : LedState::ON);         
        }

        void updateState(LedState state) {
            this->state = state;
            if (state == LedState::ON) {
              if (pwwControl != nullptr) {
                pwwControl->pulse();  
              } else {
              digitalWrite(this->ledPin, HIGH);
              }
            } else {
              digitalWrite(this->ledPin, LOW);
            }
        }

        LedState getState(){
            return this->state;
        }    
  };

  PWMControl pwwControlGreen(LedWorkConfig::PWW_LED_FREQUENCY_GZ, micros);  
  PWMControl pwwControlYellow(LedWorkConfig::PWW_LED_FREQUENCY_GZ, micros);
  PWMControl pwwControlRed(LedWorkConfig::PWW_LED_FREQUENCY_GZ, micros);

  Led ledGreen(LedWorkConfig::LED_PIN_GREEN, &pwwControlGreen);
  Led ledYellow(LedWorkConfig::LED_PIN_YELLOW, &pwwControlYellow);
  Led ledRed(LedWorkConfig::LED_PIN_RED, &pwwControlRed);
  

  volatile long period_count_milisec = 0;

  void IRAM_ATTR onTimer() {      
    period_count_milisec++;  
    if (period_count_milisec >= LedWorkConfig::TRAFIC_ALL_LIGHT_WORK_PERIOD_MILISEC) {
      period_count_milisec = 0;
    }
  }

  void setup() {  

      pwwControlGreen.setPulseFunc(greenOn, greenOff);
      pwwControlRed.setPulseFunc(redOn, redOff);
      pwwControlYellow.setPulseFunc(yellowOn, yellowOff);

      analogSetAttenuation(ADC_11db);    
      hw_timer_t* timer = timerBegin(0, 80, true); // таймер на ESP32
      timerAttachInterrupt(timer, onTimer, true);
      timerAlarmWrite(timer, 1000, true);          // кожну mil sec секунду 000
      timerAlarmEnable(timer);
      Serial.begin(115200);        
  }

  int getMode()
  {
    int duration = 0;
    for(int period = 0; period <= RED_YELOW_LIGHT; ++period) {
      duration += PhaseDurations[period];
      if (period_count_milisec < duration) { 
        return period;
      }
    }
    return GREEN_LIGHT;
  }

  void loop() {            
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

            if ((currentPhaseMilisec % blinkPeriod) < (blinkPeriod / 2)) {
                ledGreen.updateState(LedState::OFF);
            } else {
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

// Зелёный светодиод
void greenOn() {
    digitalWrite(LedWorkConfig::LED_PIN_GREEN, HIGH);
}
void greenOff() {
    digitalWrite(LedWorkConfig::LED_PIN_GREEN, LOW);
}
// Жёлтый светодиод
void yellowOn() {
    digitalWrite(LedWorkConfig::LED_PIN_YELLOW, HIGH);
}
void yellowOff() {
    digitalWrite(LedWorkConfig::LED_PIN_YELLOW, LOW);
}
// Красный светодиод
void redOn() {
    digitalWrite(LedWorkConfig::LED_PIN_RED, HIGH);
}
void redOff() {
    digitalWrite(LedWorkConfig::LED_PIN_RED, LOW);
}
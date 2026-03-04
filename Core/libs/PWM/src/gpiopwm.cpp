#include "gpiopwm.h"

    
      GPIOPWMControl::GPIOPWMControl(IGPIO* gpio, int pwwFrequencyGZ, GetMicrosecFunc microseconds) :             
            gpio(gpio), microsecFunc(microseconds), workFrequencyGZ(pwwFrequencyGZ) {      
        this->pwwPeriodInMilisec =  (1 / static_cast<double>(pwwFrequencyGZ)) * 1000.0;
        this->duteLightTimeInMilisec = this->pwwPeriodInMilisec;
        this->duteLastTimeMicros = 0;               
      }
      
      //void PWMControl::setPulseFunc(Func dutyFunc, Func offFunc) {        //this->dutyFunc = dutyFunc; //this->offFunc = offFunc;    }  

      void GPIOPWMControl::setPWWDuteTime(int analogResolutionRange, int currentResolution) {        
        double duteTimeMilisec = (static_cast<double>(currentResolution) / static_cast<double>(analogResolutionRange)) * this->pwwPeriodInMilisec;
        this->duteLightTimeInMilisec =  duteTimeMilisec;    
      }      

      void GPIOPWMControl::pulse() {      
        unsigned long currentTime = microsecFunc(); // точность до микросекунд
        unsigned long elapsed = currentTime - this->duteLastTimeMicros;

        // Переводим миллисекунды в микросекунды для сравнения
        double duteTimeMicros = this->duteLightTimeInMilisec * 1000.0;
        double periodMicros   = this->pwwPeriodInMilisec * 1000.0;

        if (elapsed < duteTimeMicros) {
            gpio->writeGPIO(true);
            //digitalWrite(this->ledPin, HIGH);
        } else if (elapsed < periodMicros) {
            gpio->writeGPIO(false);
            //digitalWrite(this->ledPin, LOW);
         } else {
           this->duteLastTimeMicros = currentTime; // начинаем новый цикл
         }         
      }      

      IGPIO* GPIOPWMControl::getGPIO() {
        return this->gpio;
      }


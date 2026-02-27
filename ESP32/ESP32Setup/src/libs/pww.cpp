typedef unsigned long (*GetMicrosecFunc)(void);
typedef void (*Func)(void);

class PWMControl {  
  private:       
      int workFrequencyGZ;
      double pwwPeriodInMilisec;       
      double duteLightTimeInMilisec;
      long duteLastTimeMicros;
      GetMicrosecFunc microsecFunc;
      Func dutyFunc;
      Func offFunc;

  public:
      PWMControl() {

      } 

      PWMControl(int pwwFrequencyGZ, GetMicrosecFunc microseconds) : 
            dutyFunc(dutyFunc), offFunc(offFunc),
            microsecFunc(microseconds), workFrequencyGZ(pwwFrequencyGZ) {      
        this->pwwPeriodInMilisec =  (1 / static_cast<double>(pwwFrequencyGZ)) * 1000.0;
        this->duteLightTimeInMilisec = this->pwwPeriodInMilisec;
        this->duteLastTimeMicros = 0;               
      }
      
      void setPulseFunc(Func dutyFunc, Func offFunc) {
        this->dutyFunc = dutyFunc; 
        this->offFunc = offFunc;    
      }  

      void setPWWDuteTime(int analogResolutionRange, int currentResolution) {        
        double duteTimeMilisec = (static_cast<double>(currentResolution) / static_cast<double>(analogResolutionRange)) * this->pwwPeriodInMilisec;
        this->duteLightTimeInMilisec =  duteTimeMilisec;    
      }      

      void pulse() {
      
        unsigned long currentTime = microsecFunc(); // точность до микросекунд
        unsigned long elapsed = currentTime - this->duteLastTimeMicros;

        // Переводим миллисекунды в микросекунды для сравнения
        double duteTimeMicros = this->duteLightTimeInMilisec * 1000.0;
        double periodMicros   = this->pwwPeriodInMilisec * 1000.0;

        if (elapsed < duteTimeMicros && dutyFunc != nullptr) {
            dutyFunc();
            //digitalWrite(this->ledPin, HIGH);
        } else if (offFunc != nullptr && elapsed < periodMicros) {
            offFunc();
            //digitalWrite(this->ledPin, LOW);
         } else {
           this->duteLastTimeMicros = currentTime; // начинаем новый цикл
         }         
      }      
};


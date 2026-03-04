#ifndef LED_H
#define LED_H   
#include "gpiopwm.h"

enum class LedState {
    ON, OFF
};

class Led {  
    private:        
        LedState state;     
        GPIOPWMControl* gpioPwmControl;

    public:        
        Led(GPIOPWMControl* pwwControl);
        void changeState();
        void updateState(LedState state);
        LedState getState();
  };
#endif
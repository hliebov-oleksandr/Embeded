#include "led.h"

Led::Led(GPIOPWMControl *pwwControl)
{
    this->gpioPwmControl = pwwControl;
}

void Led::changeState()
{
    updateState(this->state == LedState::ON ? LedState::OFF : LedState::ON);
}

void Led::updateState(LedState state)
{
    this->state = state;
    if (state == LedState::ON)
    {
        gpioPwmControl->pulse();
    }
    else
    {
        gpioPwmControl->getGPIO()->writeGPIO(false);
    }
}

LedState Led::getState()
{
    return this->state;
}

#include "encoder.h"

Encoder::Encoder(IGPIO *gpioA, IGPIO *gpioB, IGPIO *gpioClick, MicrosecProvider microsec_h,
                 int64_t debounce, int64_t longClick)
    : gpioA(gpioA), gpioB(gpioB), gpioClick(gpioClick), microsec_h(microsec_h),
      debounceDurationMicrosec(debounce), longClickMicrosec(longClick),
      lastABState(0), phase(0), lastClickTime(0), lastDebounceTime(0)
{
    result = {0, false, ENCODER_DIRECTION_TYPE::NONE, ENCODER_CLICK_TYPE::NONE};
}

EncoderResult Encoder::readEncoder()
{
    // --- Rotary encoder part ---
    bool a = gpioA->readGPIO();
    bool b = gpioB->readGPIO();
    int newState = (a << 1) | b;

    static const int8_t transitionTable[4][4] = {
        {0, +1, -1, 0}, // old=00
        {-1, 0, 0, +1}, // old=01
        {+1, 0, 0, -1}, // old=10
        {0, -1, +1, 0}  // old=11
    };

    int delta = transitionTable[lastABState][newState];
    phase += delta;

    if (phase == 2)
    { // полный цикл вправо
        result.counter++;
        result.encoderChanged = true;
        result.direction = ENCODER_DIRECTION_TYPE::RIGHT_INCREASE;
        phase = 0;
    }
    else if (phase == -2)
    { // полный цикл влево
        result.counter--;
        result.encoderChanged = true;
        result.direction = ENCODER_DIRECTION_TYPE::LEFT_DICREASE;
        phase = 0;
    }
    else
    {
        result.encoderChanged = false;
        result.direction = ENCODER_DIRECTION_TYPE::NONE;
    }

    lastABState = newState;
    // --- Click part ---
    bool click = gpioClick->readGPIO(); // LOW = нажата
    int64_t now = microsec_h();

    if (!click)
    {
        // кнопка нажата
        if (lastClickTime == 0)
        {
            lastClickTime = now;
            longClickHandled = false; // сброс при новом нажатии
        }
        // проверяем удержание
        if (!longClickHandled && (now - lastClickTime) > longClickMicrosec)
        {
            result.clickType = ENCODER_CLICK_TYPE::LONGCLICKED;
            result.encoderChanged = true;
            longClickHandled = true; // больше не сработает до отпускания
        }
    }
    else
    {
        // кнопка отпущена
        if (lastClickTime != 0 && (now - lastDebounceTime > debounceDurationMicrosec))
        {
            lastDebounceTime = now;
            if ((now - lastClickTime) <= longClickMicrosec)
            {
                result.clickType = ENCODER_CLICK_TYPE::CLICKED;
                result.encoderChanged = true;
            }
            lastClickTime = 0; // сброс
        }
        else
        {
            result.clickType = ENCODER_CLICK_TYPE::NONE;
        }
    }
    return result;
}
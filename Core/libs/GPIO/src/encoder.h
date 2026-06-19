#ifndef ENCODER
#define ENCODER

#include "gpio.h"
#include "utils.h"

enum class ENCODER_CLICK_TYPE {
    NONE,
    CLICKED,
    LONGCLICKED
};

enum class ENCODER_DIRECTION_TYPE {
    NONE,
    LEFT_DICREASE,
    RIGHT_INCREASE
};

struct EncoderResult {
public:
    int counter;
    bool encoderChanged;
    ENCODER_DIRECTION_TYPE direction;
    ENCODER_CLICK_TYPE clickType;
};

class Encoder {
private:
    EncoderResult result;

    IGPIO* gpioA;
    IGPIO* gpioB;    
    IGPIO* gpioClick;    
    MicrosecProvider microsec_h;    

    int64_t debounceDurationMicrosec;
    int64_t longClickMicrosec;
    int lastABState = 0;
    int phase = 0;                 // для подсчёта полного цикла
    int64_t lastClickTime = 0;
    int64_t lastDebounceTime = 0;
    bool longClickHandled = false;

public:
    Encoder(IGPIO* gpioA, IGPIO* gpioB, IGPIO* gpioClick, MicrosecProvider microsec_h,
            int64_t debounce = 50, int64_t longClick = 500000);

    EncoderResult readEncoder();
};

#endif
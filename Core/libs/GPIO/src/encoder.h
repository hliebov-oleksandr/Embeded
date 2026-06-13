#ifndef ENCODER
#define ENCODER
#include "gpio.h"

enum class ENCODER_SWITCH_TYPE
{
    NONE,
    CLICKED,
    LONGCLICKED
};

enum class ENCODER_TYPE
{
    NONE,
    A_SWICH,
    B_SWICH
};

enum class ENCODER_DIRECTION_TYPE {
    NONE,
    LEFT_DICREASE,
    RIGHT_INCREASE
};

struct EncoderResult
{
   public:
   int counter;
   bool encoderChanged;
   ENCODER_DIRECTION_TYPE direction;
   ENCODER_SWITCH_TYPE switchType;
};

class Encoder
{
private:
    EncoderResult result;
    bool isAside = false;
    bool isBside = false;
    ENCODER_TYPE lastSwichSelection = ENCODER_TYPE::NONE;

    IGPIO* gpioA;
    IGPIO* gpioB;    
    /* data */
public:
   Encoder(IGPIO* gpioA, IGPIO* gpioB);
   EncoderResult readEncoder();
};

#endif
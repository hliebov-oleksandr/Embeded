#ifndef ENCODER
#define ENCODER
#include "gpio.h"

enum class ENCODER_TYPE
{
    NONE,
    A_SWICH,
    B_SWICH
};

enum class EncoderDirection {
    NONE,
    LEFT_DICREASE,
    RIGHT_INCREASE
};

struct EncoderResult
{
   public:
   int counter;
   bool encoderChanged;
   EncoderDirection direction;
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
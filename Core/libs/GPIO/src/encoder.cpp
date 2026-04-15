#include "encoder.h"

Encoder::Encoder(IGPIO* gpioA, IGPIO* gpioB) : gpioA(gpioA), gpioB(gpioB)
{
    result = {
        .counter = 0,
        .encoderChanged = false,
        .direction = EncoderDirection::NONE
    };
}

EncoderResult Encoder::readEncoder()
{
        int a = gpioA->readGPIO();
        int b = gpioB->readGPIO();
        result.encoderChanged = false;

        if (!a && b)
        {
            isAside = true;
            //printf("Encoder+ A: %d B: %d C: %d \n", a, b, count);
            lastSwichSelection = ENCODER_TYPE::A_SWICH;
            // printf("Encoder A: %d\n", count);
        }
        else if (!b && a)
        {
            isBside = true;
            //printf("Encoder - A: %d B: %d  C: %d\n", a, b, count);
            lastSwichSelection = ENCODER_TYPE::B_SWICH;
            // printf("Encoder B: %d\n", count);
        }

        if (a && b && (isAside || isBside))
        {
            if (isAside && !isBside)
            {
                result.counter++;
            }
            else if (isBside && !isAside)
            {
                result.counter--;
            }
            else if (isAside && isBside && lastSwichSelection == ENCODER_TYPE::B_SWICH)
            {
                result.counter++;
            }
            else if (isAside && isBside && lastSwichSelection == ENCODER_TYPE::A_SWICH)
            {
                result.counter--;
            }
            isAside = isBside = false;
            lastSwichSelection = ENCODER_TYPE::NONE;
            result.encoderChanged = true;
            //printf("Encoder A: %d B: %d C: %d \n", a, b, result.counter);            
        }

        return result;
}

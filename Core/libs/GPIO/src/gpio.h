#ifndef GPIO_H
#define GPIO_H

class IGPIO
{
public:
    virtual void writeGPIO(bool levelHigh) = 0;
    virtual int readGPIO() = 0;
    virtual int analogReqad()=0;    
    virtual ~IGPIO() = default;
};

#endif
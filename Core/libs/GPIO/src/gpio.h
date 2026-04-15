#ifndef GPIO_H
#define GPIO_H

enum class PulUPDonw {
    PULL_UP,
    PULL_DOWN
};

class IGPIO
{
public:
    virtual void writeGPIO(bool levelHigh) = 0;
    virtual int readGPIO() = 0;
    virtual int analogRead() = 0;    
    virtual void pullUpDown(PulUPDonw type) = 0;
    virtual ~IGPIO() = default;
};

#endif
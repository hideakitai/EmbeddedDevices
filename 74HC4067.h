#pragma once
#include "PCA9536.h"

class 74HC4067
{
    #define MUX_PORT_SIZE 16
    PCA9536 ioex;
    const uint8_t pin_z;


public:
    74HC4067()
    : pin_z(PIN_MUX_OUT)
    {

    }
    void setup()
    {
        pinMode(pin_z, INPUT_PULLDOWN);
        ioex.setup();
    }

    uint8_t getPort(uint8_t port)
    {
        ioex.output(port);
        return (uint8_t)digitalRead(pin_z);
    }
};

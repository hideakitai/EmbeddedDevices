#pragma once
#include "IoExPCA9536.h"

class Mux74HC4067
{
    #define MUX_PORT_SIZE 16
    IoExPCA9536 ioex;
    const uint8_t pin_z;


public:
    Mux74HC4067()
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

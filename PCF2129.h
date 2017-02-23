#pragma once
#include <i2c_t3.h>

class RtcPCF2129
{
    enum RTC_REG
    {
        REG_CONTROL1 = 0x00,
        REG_CONTROL2 = 0x01,
        REG_CONTROL3 = 0x02,
        REG_WATCHDG_TIM_CTL = 0x10
    };
    enum RTC_REG_VALUE
    {
        REG_CONTROL1_STOP_0 = 0x09,
        REG_CONTROL1_STOP_1 = 0x29,
        REG_WATCHDG_TIM_CTL_TI_TP_0 = 0x03
    };
    const uint8_t RTC_ADDR;
public:

    const uint8_t pin_INT;
    volatile bool b_interrupt;

    RtcPCF2129(uint8_t pin_int)
    : RTC_ADDR(0x51)
    , pin_INT(pin_int)
    {
    }

    void setup()
    {
        pinMode(pin_INT, INPUT); // THIS SHOULD NOT BE "INPUT_PULLUP"
        Wire.begin();

        Wire.beginTransmission(RTC_ADDR);
        Wire.write(REG_WATCHDG_TIM_CTL);
        Wire.write(REG_WATCHDG_TIM_CTL_TI_TP_0); // default : bit5 TI_TP = 0
        Wire.endTransmission();
        stop();
    }

    void start()
    {
        Wire.beginTransmission(RTC_ADDR);
        Wire.write(REG_CONTROL1);
        Wire.write(REG_CONTROL1_STOP_0); // set watchdog timer to generate pulse / second, and stop rtc
        Wire.endTransmission();
        resumeInterrupt();
    }

    void stop()
    {
        Wire.beginTransmission(RTC_ADDR);
        Wire.write(REG_CONTROL1);
        Wire.write(REG_CONTROL1_STOP_1); // set watchdog timer to generate pulse / second, and stop rtc
        Wire.endTransmission();
        resumeInterrupt();
    }

    uint8_t getIntPin() { return pin_INT; }
    void setInterrupted() { b_interrupt = true; }
    void resumeInterrupt()
    {
        Wire.beginTransmission(RTC_ADDR);
        Wire.write(REG_CONTROL2);
        Wire.write(0x00); // clear MSF flag
        Wire.endTransmission();
        b_interrupt = false;
    }
    bool isInterrupted() { return b_interrupt; }

    uint8_t read(uint8_t reg)
    {
        Wire.beginTransmission(RTC_ADDR);
        Wire.write(reg);
        Wire.endTransmission();

        Wire.requestFrom(RTC_ADDR, 1, I2C_STOP);  // blocking read (request 256 bytes)

        if (Wire.available()) return Wire.readByte();
        else                  return 0x00;
    }
};

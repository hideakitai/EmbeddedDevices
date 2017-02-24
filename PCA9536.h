#pragma once
#include <i2c_t3.h>

class PCA9536
{
    enum IOEX_REG
    {
        IOEX_REG_INPUT = 0x00,
        IOEX_REG_OUTPUT,
        IOEX_REG_INVERT,
        IOEX_REG_CONFIG
    };

    enum IOEX_DIR
    {
        IOEX_DIR_OUTPUT = 0x00,
        IOEX_DIR_INPUT = 0x01,
        IOEX_DIR_OUTPUT_ALL = 0xF0,
        IOEX_DIR_INPUT_ALL = 0xFF
    };

    enum IOEX_OUTPUT_LEVEL
    {
        IOEX_OUTPUT_L = 0x00,
        IOEX_OUTPUT_H = 0x01,
        IOEX_OUTPUT_L_ALL = 0x00,
        IOEX_OUTPUT_H_ALL = 0x0F
    };

    #define IOEX_ADDR       0x41
    #define IOEX_REG_SIZE   0x04
    #define IOEX_PORT_SIZE  0x04

    uint8_t ioex_reg_status[IOEX_REG_SIZE];

public:


    PCA9536()
    : ioex_reg_status({0xFF})
    {}

    ~PCA9536()
    {}

    void setup()
    {
        Wire.begin();
        for (size_t i = 0; i < IOEX_REG_SIZE; ++i) setIO(IOEX_DIR_OUTPUT);
    }

    void setIO(IOEX_DIR dir)
    {
        Wire.beginTransmission(IOEX_ADDR);
        Wire.write(IOEX_REG_CONFIG);
        if (dir == IOEX_DIR_OUTPUT) Wire.write(IOEX_DIR_OUTPUT_ALL);
        else                        Wire.write(IOEX_DIR_INPUT_ALL);
        Wire.endTransmission();

        read(IOEX_REG_CONFIG);
    }

    void output(IOEX_OUTPUT_LEVEL level)
    {
        Wire.beginTransmission(IOEX_ADDR);
        Wire.write(IOEX_REG_OUTPUT);
        if (level == IOEX_OUTPUT_H) Wire.write(IOEX_OUTPUT_H_ALL);
        else                        Wire.write(IOEX_OUTPUT_L_ALL);
        Wire.endTransmission();

        read(IOEX_REG_OUTPUT);
    }

    void output(uint8_t bits)
    {
        Wire.beginTransmission(IOEX_ADDR);
        Wire.write(IOEX_REG_OUTPUT);
        Wire.write(bits);
        Wire.endTransmission();

        read(IOEX_REG_OUTPUT);
    }

    void output(uint8_t port, IOEX_OUTPUT_LEVEL level)
    {
        if (port >= IOEX_PORT_SIZE) return;
        Wire.beginTransmission(IOEX_ADDR);
        Wire.write(IOEX_REG_OUTPUT);
        Wire.write((uint8_t)(level << port));
        Wire.endTransmission();

        read(IOEX_REG_OUTPUT);
    }

    void read(IOEX_REG reg)
    {
        Wire.beginTransmission(IOEX_ADDR);
        Wire.write(reg);
        Wire.endTransmission();

        Wire.requestFrom(IOEX_ADDR, 1, I2C_STOP);  // blocking read (request 256 bytes)

        while(Wire.available()) ioex_reg_status[reg] = Wire.readByte();
    }

    void read()
    {
        for (size_t i = 0; i < IOEX_REG_SIZE; ++i) read((IOEX_REG)i);
    }

    void print(IOEX_REG reg)
    {
        Serial.printf("IOEX REG %d : %02x\n", reg, ioex_reg_status[reg]);
    }

    void print()
    {
        for (size_t i = 0; i < IOEX_REG_SIZE; ++i) print((IOEX_REG)i);
    }
};

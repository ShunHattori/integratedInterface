#pragma once

#include <stdint.h>
#include <Print.h>

#define AQM0802_I2C_DEFAULT_ADDR 0x3e

// commands
#define FunctionSet1 0x38
#define FunctionSet2 0x39
#define InternalOSCFrequency 0x14
#define ContrastSet 0x70
#define PowerControl 0x56
#define FollowerControl 0x6C
#define DisplayOn 0x40
#define ClearDisplay 0x80

class AQM0802 : public Print
{
public:
    AQM0802(){};

    void init(uint8_t contrast);
    void setContrast(uint8_t contrast);
    void setCursor(uint8_t col, uint8_t row);
    void clear();

private:
    virtual size_t write(uint8_t value);
    void command(uint8_t value);
};

extern AQM0802 LCD;

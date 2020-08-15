#pragma once
#include <Arduino.h>
#include "../lib/ComponentPropaties.hpp"

class AnalogIn
{

private:
    AnalogInPropaties ANALOG_IN_PROPS;

public:
    AnalogIn() = delete;
    AnalogIn(AnalogInPropaties);
    virtual ~AnalogIn(){};

    uint16_t getValue();
};

inline AnalogIn::AnalogIn(AnalogInPropaties ANALOG_IN_PROPS_) : ANALOG_IN_PROPS(ANALOG_IN_PROPS_){};

inline uint16_t AnalogIn::getValue()
{
    return analogRead(ANALOG_IN_PROPS.pin);
}

extern AnalogIn analog_in1;
extern AnalogIn analog_in2;
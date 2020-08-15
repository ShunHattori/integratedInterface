#pragma once
#include "Arduino.h"
#include "../lib/ComponentPropaties.hpp"

inline void pin_initialize()
{
    pinMode(SWITCH_ITFC1_PROPS.pin, INPUT);
    pinMode(SWITCH_ITFC2_PROPS.pin, INPUT);
    pinMode(SWITCH_ITFC3_PROPS.pin, INPUT);
    pinMode(SWITCH_ITFC4_PROPS.pin, INPUT);
    pinMode(SWITCH_ITFC5_PROPS.pin, INPUT);
    pinMode(SWITCH_ITFC6_PROPS.pin, INPUT);
    pinMode(SWITCH_MACH1_PROPS.pin, INPUT);
    pinMode(SWITCH_MACH2_PROPS.pin, INPUT);
    pinMode(SWITCH_MACH3_PROPS.pin, INPUT);
    pinMode(SWITCH_MACH4_PROPS.pin, INPUT);
    pinMode(SWITCH_MACH5_PROPS.pin, INPUT);
    pinMode(SWITCH_MACH6_PROPS.pin, INPUT);
    pinMode(EMERGENCY_PROPS.current_sensor_pin, INPUT);
    pinMode(EMERGENCY_PROPS.emergency_signal_pin, OUTPUT);
    pinMode(ANALOGIN1_PROPS.pin, INPUT);
    pinMode(ANALOGIN2_PROPS.pin, INPUT);
    return;
}
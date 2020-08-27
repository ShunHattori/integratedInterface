#pragma once

#include "Arduino.h"
#include "../lib/ComponentPropaties.hpp"
#include "../lib/PWMFrequency.hpp"

class MotorDriver
{
private:
    MotorPropaties motor_props;
    int16_t pwm;

public:
    MotorDriver() = delete;
    MotorDriver(MotorPropaties);
    virtual ~MotorDriver() {}

    void set_pwm(int16_t _pwm);
    void apply_pwm();
};

inline MotorDriver::MotorDriver(MotorPropaties motor_props_) : motor_props(motor_props_)
{
    setPwmFrequencyMEGA2560(motor_props.cw_pin, 1);
    setPwmFrequencyMEGA2560(motor_props.ccw_pin, 1);
};

inline void MotorDriver::set_pwm(int16_t _pwm)
{
    pwm = constrain(_pwm, -motor_props.max_pwm, motor_props.max_pwm);
}

inline void MotorDriver::apply_pwm()
{
    uint8_t pwm_cw = 0 < pwm ? pwm : 0, pwm_ccw = pwm < 0 ? -pwm : 0;
    analogWrite(motor_props.cw_pin, pwm_cw);
    analogWrite(motor_props.ccw_pin, pwm_ccw);
}

extern MotorDriver motor1;
extern MotorDriver motor2;
extern MotorDriver motor3;
extern MotorDriver motor4;

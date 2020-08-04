#pragma once
#include "Arduino.h"
#include "ComponentPropaties.hpp"

class MotorDriver
{
private:
    MotorPropaties motor_pros;
    int16_t pwm;

public:
    MotorDriver() = delete;
    MotorDriver(MotorPropaties motor_props_)
        : motor_pros(motor_props_){};

    //TODO:sourseに移行させる
    void set_pwm(int16_t _pwm)
    {
        pwm = constrain(_pwm, -motor_pros.max_pwm, motor_pros.max_pwm);
    }

    void apply_pwm()
    {
        uint8_t pwm_cw = 0 < pwm ? pwm : 0,
                pwm_ccw = pwm < 0 ? -pwm : 0;
        analogWrite(motor_pros.cw_pin, pwm_cw);
        analogWrite(motor_pros.ccw_pin, pwm_ccw);
        Serial.print(pwm_cw);
        Serial.print('\t');
        Serial.print(pwm_ccw);
        Serial.print("\r\n");
    }

    virtual ~MotorDriver() {}
};

MotorDriver motor1(MOTOR1_PROPS);
MotorDriver motor2(MOTOR2_PROPS);
MotorDriver motor3(MOTOR3_PROPS);
MotorDriver motor4(MOTOR4_PROPS);
#pragma once

#include <Arduino.h>
#include <Servo.h>
#include "../lib/ComponentPropaties.hpp"

class ServoDriver
{
private:
    Servo SERVO;
    ServoPropaties SERVO_PROPS;

public:
    ServoDriver() = delete;
    ServoDriver(ServoPropaties);
    virtual ~ServoDriver() {}

    void init();
    void setAngle(int16_t);
    int8_t getAngle();
};

inline ServoDriver::ServoDriver(ServoPropaties SERVO_PROPS_) : SERVO_PROPS(SERVO_PROPS_)
{
    //SERVO.attach(SERVO_PROPS.pin, SERVO_PROPS.pwm_width_min, SERVO_PROPS.pwm_width_max);
}
inline void ServoDriver::init()
{
    SERVO.attach(SERVO_PROPS.pin);
}

inline void ServoDriver::setAngle(int16_t angle)
{
    SERVO.write(angle);
    return;
}

inline int8_t ServoDriver::getAngle()
{
    return SERVO.read();
}

extern ServoDriver servo1;
extern ServoDriver servo2;
extern ServoDriver servo3;
extern ServoDriver servo4;
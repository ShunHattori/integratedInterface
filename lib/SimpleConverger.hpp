#pragma once
#include "Arduino.h"
#include "ComponentPropaties.hpp"

class SimpleConverger
{
private:
    int16_t target_position, current_position, pwm_output;
    ConvergerPropaties converger_props;

public:
    SimpleConverger() = delete;
    SimpleConverger(ConvergerPropaties converger_props_)
        : converger_props(converger_props_){};
    virtual ~SimpleConverger(){};

    void update();
    void setTarget(int16_t);
    void setCurrent(int16_t);
    int16_t getPwm();
};

inline void SimpleConverger::update()
{
    int16_t error = target_position - current_position;
    float pwm_1 = error * converger_props.pGain;
    float pwm_constrained = constrain(pwm_1, -converger_props.max_pwm, converger_props.max_pwm);
    if (-converger_props.min_pwm < pwm_constrained && pwm_constrained < converger_props.min_pwm)
    {
        if (pwm_constrained < 0)
            pwm_output = -converger_props.min_pwm;
        else
            pwm_output = converger_props.min_pwm;
    }
    else if (pwm_constrained < -converger_props.max_pwm || converger_props.max_pwm < pwm_constrained)
    {
        if (pwm_constrained < 0)
            pwm_output = -converger_props.max_pwm;
        else
            pwm_output = converger_props.max_pwm;
    }
    else
    {
        pwm_output = pwm_constrained;
    }
    return;
}
inline void SimpleConverger::setTarget(int16_t target_)
{
    target_position = target_;
    return;
}
inline void SimpleConverger::setCurrent(int16_t current_)
{
    current_position = current_;
    return;
}
inline int16_t SimpleConverger::getPwm()
{
    return pwm_output;
}
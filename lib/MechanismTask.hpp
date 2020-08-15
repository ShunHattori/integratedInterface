#pragma once

#include "../lib/MotorDriver.hpp"
#include "../lib/RobotFlagList.hpp"
#include "../lib/ServoDriver.hpp"
#include "../lib/AntiChattering.hpp"
#include "../lib/AnalogIn.hpp"

class MechanismTask
{
private:
    FlagSet &flag_set;
    MotorDriver MotorSet[4] = {motor1, motor2, motor3, motor4};
    void work1();
    void work2();
    void work3();
    void work4();
    void work5();

    void apply_motor_stop();
    void apply_emergency_state();
    void apply_pwms();

public:
    MechanismTask() = delete;
    MechanismTask(FlagSet &);
    virtual ~MechanismTask() {}

    void work();
};
inline MechanismTask::MechanismTask(FlagSet &flag_set_) : flag_set(flag_set_){};

inline void MechanismTask::apply_motor_stop()
{
    for (auto &&driver_target : MotorSet)
    {
        driver_target.set_pwm(0);
        driver_target.apply_pwm();
    }
    return;
}

inline void MechanismTask::apply_pwms()
{
    for (auto &&driver_target : MotorSet)
        driver_target.apply_pwm();
    return;
}

inline void MechanismTask::apply_emergency_state()
{
    if (!flag_set.sw_state_emergency)
        return;
    this->apply_motor_stop();
    return;
}

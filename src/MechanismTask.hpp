#pragma once

#include "MotorDriver.hpp"
#include "RobotFlagList.hpp"

class MechanismTask
{
public:
    MechanismTask() = delete;
    MechanismTask(FlagSet &flag_set_)
        : flag_set(flag_set_) {}

    void work()
    {
        /*----TASKS HERE----*/

        /*----TASKS HERE----*/

        /*
         *  processing at the bottom of tasks
         */
        apply_emergency_state();
        apply_pwms();
    }

    void apply_emergency_state()
    {
        if (!flag_set.sw_state_emergency)
            return;
        for (auto &&driver_target : MotorSet)
            driver_target.set_pwm(0);
        return;
    }

    void apply_pwms()
    {
        for (auto &&driver_target : MotorSet)
            driver_target.apply_pwm();
        return;
    }

    virtual ~MechanismTask()
    {
    }

private:
    FlagSet flag_set;
    MotorDriver MotorSet[4] = {motor1, motor2, motor3, motor4};
};

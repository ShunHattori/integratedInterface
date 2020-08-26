#pragma once

#include "../lib/MotorDriver.hpp"
#include "../lib/RobotFlagList.hpp"
#include "../lib/ServoDriver.hpp"
#include "../lib/AntiChattering.hpp"
#include "../lib/AnalogIn.hpp"
#include "../lib/Adafruit_NeoPixel.h"

class MechanismTask
{
private:
    FlagSet &flag_set;
    MotorDriver MotorSet[4] = {motor1, motor2, motor3, motor4};
    Adafruit_NeoPixel strip1, strip2;
    void work1();
    void work2();
    void work3();
    void work4();
    void work5();

    void apply_motor_stop();
    void apply_emergency_state();
    void apply_pwms();

    bool waitMs(uint16_t);
    void showPixels(Adafruit_NeoPixel &, uint32_t);

public:
    MechanismTask() = delete;
    MechanismTask(FlagSet &);
    virtual ~MechanismTask() {}

    void work();
};

inline MechanismTask::MechanismTask(FlagSet &flag_set_)
    : flag_set(flag_set_),
      strip1(NEOPIXEL1_PROPS.num_pixels, NEOPIXEL1_PROPS.pin, NEO_GRB + NEO_KHZ800),
      strip2(NEOPIXEL2_PROPS.num_pixels, NEOPIXEL2_PROPS.pin, NEO_GRB + NEO_KHZ800)
{
    strip1.begin();
    strip2.begin();
};

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

inline bool MechanismTask::waitMs(uint16_t times)
{
    static unsigned long long prevLaunched = 0;
    if ((millis() - prevLaunched) > times)
    {
        prevLaunched = millis();
        return 1;
    }
    return 0;
}

inline void MechanismTask::showPixels(Adafruit_NeoPixel &strip, uint32_t color)
{
    for (uint16_t i = 0; i < strip.numPixels(); i++)
    {
        strip.setPixelColor(i, color);
        strip.show();
    }
}

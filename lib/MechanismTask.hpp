#pragma once

#include "../lib/MotorDriver.hpp"
#include "../lib/RobotFlagList.hpp"
#include "../lib/ServoDriver.hpp"
#include "../lib/AntiChattering.hpp"
#include "../lib/AnalogIn.hpp"
#include "../lib/Adafruit_NeoPixel.h"
#include "../lib/AQM0802.h"
#include "../lib/Observer.hpp"
#include "../lib/SimpleConverger.hpp"

class MechanismTask
{
private:
    FlagSet &flag_set;
    Adafruit_NeoPixel strip1, strip2;
    Observer<bool> obs1, obs2, obs3, obs4, obs5, obs6, obs7;
    SimpleConverger conv1;
    void work1();
    void work2();
    void work3();
    void work4();
    void work5();

    void apply_motor_stop();
    void apply_emergency_state();
    void apply_pwms();

    bool waitMs1(uint16_t);
    bool waitMs2(uint16_t);

    void showPixels(Adafruit_NeoPixel &, uint32_t);
    void updateSwitchInputs();

public:
    MechanismTask() = delete;
    MechanismTask(FlagSet &);
    virtual ~MechanismTask() {}

    void initialize();
    void work();
};

inline MechanismTask::MechanismTask(FlagSet &flag_set_)
    : flag_set(flag_set_),
      strip1(NEOPIXEL1_PROPS.num_pixels, NEOPIXEL1_PROPS.pin, NEO_GRB + NEO_KHZ800),
      strip2(NEOPIXEL2_PROPS.num_pixels, NEOPIXEL2_PROPS.pin, NEO_GRB + NEO_KHZ800),
      conv1(CONVERGER1_PROPS){};

inline void MechanismTask::initialize()
{
    LCD.init(30);
    LCD.clear();
    LCD.setContrast(30);
    LCD.setCursor(0, 0);
    LCD.print("[");
    LCD.setCursor(7, 0);
    LCD.print("]");
    LCD.setCursor(0, 1);
    LCD.print("SEQ->0");
    servo1.init();
    servo2.init();
    servo3.init();
    servo4.init();
    strip1.begin();
    strip2.begin();
    for (uint8_t i = 0; i < strip1.numPixels(); i++)
    {
        strip1.setPixelColor(i, strip1.Color(0xFF, 0xFF, 0xFF));
    }
    for (uint8_t i = 0; i < strip2.numPixels(); i++)
    {
        strip2.setPixelColor(i, strip2.Color(0xFF, 0xFF, 0xFF));
    }
    strip1.setBrightness(255);
    strip2.setBrightness(255);
    strip1.show();
    strip2.show();
    servo1.setAngle(90);
}

inline void MechanismTask::apply_motor_stop()
{
    motor1.set_pwm(0);
    motor2.set_pwm(0);
    motor3.set_pwm(0);
    motor4.set_pwm(0);
    this->apply_pwms();
    return;
}

inline void MechanismTask::apply_pwms()
{
    motor1.apply_pwm();
    motor2.apply_pwm();
    motor3.apply_pwm();
    motor4.apply_pwm();
    return;
}

inline void MechanismTask::apply_emergency_state()
{
    if (!flag_set.sw_state_emergency)
        return;
    this->apply_motor_stop();
    return;
}

inline bool MechanismTask::waitMs1(uint16_t times)
{
    static unsigned long long prevLaunched = 0;
    if ((millis() - prevLaunched) > times)
    {
        prevLaunched = millis();
        return 1;
    }
    return 0;
}
inline bool MechanismTask::waitMs2(uint16_t times)
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

inline void MechanismTask::updateSwitchInputs()
{
    switch_i1.update();
    switch_i2.update();
    switch_i3.update();
    switch_i4.update();
    switch_i5.update();
    switch_i6.update();
    switch_m1.update();
    switch_m2.update();
    switch_m3.update();
    switch_m4.update();
    switch_m5.update();
    switch_m6.update();
}
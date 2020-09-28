#pragma once
#include "Arduino.h"

//robot_IDは1~の数字
//遠隔のどのビットを見るか、コントローラ信号での識別番号となる

constexpr uint8_t robot_ID = 1;

struct MotorPropaties
{
    uint8_t cw_pin;
    uint8_t ccw_pin;
    int16_t max_pwm;
};
constexpr MotorPropaties MOTOR1_PROPS = {2, 3, 200};
constexpr MotorPropaties MOTOR2_PROPS = {5, 6, 200};
constexpr MotorPropaties MOTOR3_PROPS = {46, 8, 200};
constexpr MotorPropaties MOTOR4_PROPS = {9, 10, 200};

struct ServoPropaties
{
    int16_t pwm_width_min, pwm_width_max;
    uint8_t pin;
};
constexpr ServoPropaties SERVO1_PROPS = {0, 2400, 11};
constexpr ServoPropaties SERVO2_PROPS = {0, 2400, 12};
constexpr ServoPropaties SERVO3_PROPS = {0, 2400, 44};
constexpr ServoPropaties SERVO4_PROPS = {0, 2400, 45};

struct SwitchPropaties
{
    bool is_analog_pin,
        is_external_pullupped;
    uint8_t pin;
};
struct AnalogSwitchPropaties
{
    SwitchPropaties SwitchParam;

    uint16_t analog_threshold;
    bool state;
};
constexpr SwitchPropaties SWITCH_MACH1_PROPS = {false, false, 22};
constexpr SwitchPropaties SWITCH_MACH2_PROPS = {false, false, 23};
constexpr SwitchPropaties SWITCH_MACH3_PROPS = {false, false, 24};
constexpr SwitchPropaties SWITCH_MACH4_PROPS = {false, false, 25};
constexpr SwitchPropaties SWITCH_MACH5_PROPS = {false, false, 26};
constexpr SwitchPropaties SWITCH_MACH6_PROPS = {false, false, 27};
constexpr SwitchPropaties SWITCH_ITFC1_PROPS = {false, true, 28};
constexpr SwitchPropaties SWITCH_ITFC2_PROPS = {false, true, 29};
constexpr SwitchPropaties SWITCH_ITFC3_PROPS = {false, true, 30};
constexpr SwitchPropaties SWITCH_ITFC4_PROPS = {false, true, 31};
constexpr SwitchPropaties SWITCH_ITFC5_PROPS = {false, false, 32};
constexpr SwitchPropaties SWITCH_ITFC6_PROPS = {false, false, 33};

struct IM920Propaties
{
    char unique_id[4];
    HardwareSerial &port;
    uint32_t baud;
    uint8_t busy_pin,
        status_pin;
};
struct EmergencyPropaties
{
    IM920Propaties IM920;
    uint8_t current_sensor_pin,
        emergency_signal_pin;
    uint16_t own_signal_bit;
    int RSSIBottomLimit; //遠隔非常停止の通信強度の下限値
};
constexpr EmergencyPropaties
    EMERGENCY_PROPS = {
        {{'6', 'D', 'E', '4'}, Serial3, 19600, 40, 41},
        35,
        36,
        uint16_t(uint16_t(0b00000001) << (robot_ID - 1)),
        145,
};

struct NeopixelPropaties
{
    uint8_t pin;
    uint8_t num_pixels;
};
constexpr NeopixelPropaties NEOPIXEL1_PROPS = {4, 12};
constexpr NeopixelPropaties NEOPIXEL2_PROPS = {13, 12};

struct AnalogInPropaties
{
    uint8_t pin;
};

constexpr AnalogInPropaties ANALOGIN1_PROPS = {A0};
constexpr AnalogInPropaties ANALOGIN2_PROPS = {A1};

struct ConvergerPropaties
{
    uint8_t max_pwm, min_pwm;
    float pGain, acc_error;
};

constexpr ConvergerPropaties CONVERGER1_PROPS = {60, 30, 0.2, 5};
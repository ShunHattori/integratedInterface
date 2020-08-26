#include "../lib/MechanismTask.hpp"
#include "../lib/GeneralCalc.hpp"

MotorDriver motor1(MOTOR1_PROPS);
MotorDriver motor2(MOTOR2_PROPS);
MotorDriver motor3(MOTOR3_PROPS);
MotorDriver motor4(MOTOR4_PROPS);
ServoDriver servo1(SERVO1_PROPS);
ServoDriver servo2(SERVO2_PROPS);
ServoDriver servo3(SERVO3_PROPS);
ServoDriver servo4(SERVO4_PROPS);
AntiChattering switch_i1(SWITCH_ITFC1_PROPS);
AntiChattering switch_i2(SWITCH_ITFC2_PROPS);
AntiChattering switch_i3(SWITCH_ITFC3_PROPS);
AntiChattering switch_i4(SWITCH_ITFC4_PROPS);
AntiChattering switch_i5(SWITCH_ITFC5_PROPS);
AntiChattering switch_i6(SWITCH_ITFC6_PROPS);
AntiChattering switch_m1(SWITCH_MACH1_PROPS);
AntiChattering switch_m2(SWITCH_MACH2_PROPS);
AntiChattering switch_m3(SWITCH_MACH3_PROPS);
AntiChattering switch_m4(SWITCH_MACH4_PROPS);
AntiChattering switch_m5(SWITCH_MACH5_PROPS);
AntiChattering switch_m6(SWITCH_MACH6_PROPS);
AnalogIn analog_in1(ANALOGIN1_PROPS);
AnalogIn analog_in2(ANALOGIN2_PROPS);

void MechanismTask::work()
{
    /*****TEST CODE HERE*****/
    static uint8_t current_sequence = 0, test_pwm = 50;
    if (waitMs(2000))
    {
        current_sequence++;
    }
    switch (current_sequence)
    {
    case 1:
        motor1.set_pwm(test_pwm);
        motor2.set_pwm(test_pwm);
        motor3.set_pwm(test_pwm);
        motor4.set_pwm(test_pwm);
        showPixels(strip1, strip1.Color(0xFF, 0xFF, 0xFF));
        showPixels(strip2, strip2.Color(0xFF, 0xFF, 0xFF));
        break;
    case 2:
        motor1.set_pwm(-test_pwm);
        motor2.set_pwm(-test_pwm);
        motor3.set_pwm(-test_pwm);
        motor4.set_pwm(-test_pwm);
        showPixels(strip1, strip1.Color(0x00, 0xFF, 0xFF));
        showPixels(strip2, strip2.Color(0x00, 0xFF, 0xFF));
        break;
    case 3:
        apply_motor_stop();
        servo1.setAngle(180);
        servo2.setAngle(180);
        servo3.setAngle(180);
        servo4.setAngle(180);
        showPixels(strip1, strip1.Color(0xFF, 0x00, 0xFF));
        showPixels(strip2, strip2.Color(0xFF, 0x00, 0xFF));
        break;
    case 4:
        servo1.setAngle(0);
        servo2.setAngle(0);
        servo3.setAngle(0);
        servo4.setAngle(0);
        showPixels(strip1, strip1.Color(0xFF, 0xFF, 0x00));
        showPixels(strip2, strip2.Color(0xFF, 0xFF, 0x00));
        break;
    default:
        apply_motor_stop();
        strip1.clear();
        strip2.clear();
        strip1.show();
        strip2.show();
        servo1.setAngle(0);
        servo2.setAngle(0);
        servo3.setAngle(0);
        servo4.setAngle(0);
        break;
    }

    return;
    /************************/

    /*----TASKS HERE----*/
    constexpr int16_t motor1_pwm = 50,
                      motor2_pwm = 50,
                      servo1_angle = 90,
                      pot_target = 250,
                      pot_acc_error = 10;
    static uint8_t sequence_num = 0;

    if (flag_set.sw_state_phase1 || !flag_set.is_controller_targeted) //stop switch
    {
        apply_motor_stop();
        return;
    }

    switch (sequence_num)
    {
    case 0:
        if (flag_set.sw_state_phase2)
        {
            motor1.set_pwm(motor1_pwm);
            sequence_num = 1;
        }
        break;

    case 1:
        if (flag_set.sw_state_phase3)
        {
            sequence_num = 2;
        }
        break;

    case 2:
        // run motor2(neck) if CT-signal2 enabled
        if (switch_m1.getState())
        {
            motor1.set_pwm(0);
            motor2.set_pwm(motor2_pwm);
            sequence_num = 3;
        }
        break;

    case 3:
        //stop the motor when pot is 1/4 rotated
        if (is_near_by(analog_in1.getValue(), pot_target, pot_acc_error))
        {
            motor2.set_pwm(0);
            sequence_num = 4;
        }
        break;

    case 4:
        //blink eyes LEDs
        sequence_num = 5;
        break;

    case 5:
        // shoot the egg
        servo1.setAngle(servo1_angle);
        sequence_num = 6;
        break;

    case 6:
        //initialize for next launch
        break;
    default:
        break;
    }

    /*----TASKS HERE----*/

    /*
     *  processing at the bottom of tasks
     */
    apply_emergency_state();
    apply_pwms();
}

void work1()
{
}
void work2()
{
}
void work3()
{
}
void work4()
{
}
void work5()
{
}
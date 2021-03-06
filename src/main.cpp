#include <Arduino.h>

#include "../lib/MechanismTask.hpp"
#include "../lib/RemoteEmergency.hpp"
#include "../lib/RobotFlagList.hpp"
#include "../lib/Robot.hpp"
#include "../lib/AntiChattering.hpp"
#include "../lib/ComponentPropaties.hpp"
#include "../lib/AnalogIn.hpp"
#include "../lib/MotorDriver.hpp"
#include "../lib/ServoDriver.hpp"
#include "../lib/pinInitializer.hpp"
#include "../lib/Adafruit_NeoPixel.h"
#include "../lib/AQM0802.h"

FlagSet robot_flags;
MechanismTask robot_mechanism(robot_flags);
RemoteEmergency robot_emergency(robot_flags);
Robot obasan_1_gouki(robot_mechanism, robot_emergency);
AQM0802 LCD;

void setup()
{
    obasan_1_gouki.initialize();
}

void loop()
{
    obasan_1_gouki.work();
    //Robot::debugPrint();
}
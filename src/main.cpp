#include <Arduino.h>
#include "pinInitializer.hpp"
#include "MotorDriver.hpp"
#include "MechanismTask.hpp"
#include "RemoteEmergency.hpp"
#include "RobotFlagList.hpp"

FlagSet robot_flags;
MechanismTask robot_mechanism(robot_flags);
RemoteEmergency robot_emergency(robot_flags);

void setup()
{
  Serial.begin(256000);
  pinInitialize();
}

void loop()
{
  robot_emergency.work();
  robot_mechanism.work();
}
#pragma once

#include "../lib/pinInitializer.hpp"
#include "../lib/MechanismTask.hpp"
#include "../lib/RemoteEmergency.hpp"

class Robot
{
private:
    MechanismTask &m_task;
    RemoteEmergency &e_task;

public:
    Robot() = delete;
    Robot(MechanismTask &, RemoteEmergency &);
    virtual ~Robot() {}

    void initialize();
    uint8_t work();
};

inline Robot::Robot(MechanismTask &m_task_, RemoteEmergency &e_task_) : m_task(m_task_), e_task(e_task_){};

inline uint8_t Robot::work()
{
    m_task.work();
    e_task.work();
    return 0;
}

inline void Robot::initialize()
{
    pin_initialize();
    return;
}

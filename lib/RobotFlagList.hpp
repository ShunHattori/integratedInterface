#pragma once

struct FlagSet
{
    bool is_controller_targeted,
        sw_state_phase1,
        sw_state_phase2,
        sw_state_phase3,
        sw_state_phase4,
        sw_state_emergency,
        current_sensor;
};

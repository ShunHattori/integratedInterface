#pragma once
#include <Arduino.h>

//(value, from, to) return true when (value) is inside value from (from) to (to).
static bool is_inside_of(const int16_t, const int16_t, const int16_t);
static bool is_near_by(const int16_t, const int16_t, const uint16_t);

static bool is_inside_of(const int16_t value, const int16_t from, const int16_t to)
{
    return (((from <= value) && (value <= to)) ? true : false);
}

static bool is_near_by(const int16_t value, const int16_t base, const uint16_t error)
{
    return ((abs(base - value) <= error) ? true : false);
}
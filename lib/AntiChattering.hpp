#pragma once
#include <Arduino.h>
#include "../lib/ComponentPropaties.hpp"

/*
 *  Anti-chatter read for ATMEGA2560 controller
 */

class AntiChattering
{
private:
  SwitchPropaties SWITCH_PROPS;
  uint64_t sampled_time;
  uint16_t detect_flag_number, analog_detect_value, sampling_period, button_press_count;
  bool button_state, is_enable_updating, is_pin_analog;
  void analogProcess();
  void digitalProcess();
  void setStateFlag();

public:
  AntiChattering() = delete;
  AntiChattering(SwitchPropaties);
  void update();
  void setDetectCount(int);
  void setButtonState(bool);
  void disableStateUpdate();
  void enableStateUpdate();
  bool getState();

  virtual ~AntiChattering(){};
};

extern AntiChattering switch_i1;
extern AntiChattering switch_i2;
extern AntiChattering switch_i3;
extern AntiChattering switch_i4;
extern AntiChattering switch_i5;
extern AntiChattering switch_i6;
extern AntiChattering switch_m1;
extern AntiChattering switch_m2;
extern AntiChattering switch_m3;
extern AntiChattering switch_m4;
extern AntiChattering switch_m5;
extern AntiChattering switch_m6;

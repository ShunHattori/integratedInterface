#include "../lib/AQM0802.h"
#include "Arduino.h"
#include <Wire.h>
#if defined(__AVR__)
#include <avr/pgmspace.h>
#endif

/*********** high level commands for users***********/

void AQM0802::init(uint8_t contrast)
{
  Wire.begin();
  command(0x38);                       // function set
  command(0x39);                       // function set
  command(0x14);                       // interval osc
  command(0x70 | (contrast & 15));     // contrast low
  command(0x5c | (contrast >> 4 & 3)); // contrast high / icon / power
  command(0x6c);                       // follower control
  delay(200);
  command(0x38); // function set
  command(0x0c); // display on
  command(0x01); // clear display
}

void AQM0802::setContrast(uint8_t contrast)
{
  command(0x70 | (contrast & 15));     // contrast low
  command(0x5c | (contrast >> 4 & 3)); // contrast high / icon / power
}

void AQM0802::setCursor(uint8_t col, uint8_t row)
{
  const int row_offsets[] = {0x00, 0x40, 0x14, 0x54};

  command(0x80 | (col + row_offsets[row]));
}
void AQM0802::clear()
{
  command(0x01);
}

/*********** mid level commands, for sending data/cmds */

void AQM0802::command(uint8_t value)
{
  Wire.beginTransmission(AQM0802_I2C_DEFAULT_ADDR);
  Wire.write((uint8_t)0x00);
  Wire.write(value);
  Wire.endTransmission();
  delayMicroseconds(27); // >26.3us
}

size_t AQM0802::write(uint8_t value)
{
  Wire.beginTransmission(AQM0802_I2C_DEFAULT_ADDR);
  Wire.write((uint8_t)0x40);
  Wire.write(value);
  Wire.endTransmission();
  delayMicroseconds(27); // >26.3us

  return 1;
}

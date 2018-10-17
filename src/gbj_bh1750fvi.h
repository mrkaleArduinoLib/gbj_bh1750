/*
  NAME:
  gbj_bh1750fvi

  DESCRIPTION:
  Library for light intensity sensor BH1750FVI on two wire (I2C) bus.
  - Library enables changing address of the sensor dynamically if ADDR pin
    of the sensor is connected to a microcontroller pin and changed
    programatically accordingly as well as measurement mode.
  - Library has been inspired by the Christopher Laws's library BH1750-master
    version 1.1.3 but totally redesigned and rewritten.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the license GNU GPL v3 http://www.gnu.org/licenses/gpl-3.0.html
  (related to original code) and MIT License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_bh1750fvi.git

  CREDITS:
  TimChristopher Laws
  https://github.com/claws/BH1750.git
 */
#ifndef GBJ_BH1750FVI_H
#define GBJ_BH1750FVI_H

#include "gbj_twowire.h"


class gbj_bh1750fvi : public gbj_twowire
{
public:
//------------------------------------------------------------------------------
// Public constants
//------------------------------------------------------------------------------
static const String VERSION;
enum Addresses
{
  ADDRESS_LOW = 0x23, // ADDR <= 0.3Vcc or floating
  ADDRESS_HIGH = 0x5C,  // ADDR >= 0.7Vcc
};
enum Modes
{
  MODE_CONTINUOUS_HIGH = 0x10,  // 1 lx / 120 ms
  MODE_CONTINUOUS_HIGH2 = 0x11,  // 0.5 lx / 120 ms
  MODE_CONTINUOUS_LOW = 0x13,  // 4 lx / 16 ms
  MODE_ONETIME_HIGH = 0x20,  // 1 lx / 120 ms
  MODE_ONETIME_HIGH2 = 0x21,  // 0.5 lx / 120 ms
  MODE_ONETIME_LOW = 0x23,  // 4 lx / 16 ms
};


//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------
/*
  Constructor taken from parent class.
*/
gbj_bh1750fvi(uint32_t clockSpeed = CLOCK_100KHZ, bool busStop = true, \
  uint8_t pinSDA = 4, uint8_t pinSCL = 5) \
: gbj_twowire(clockSpeed, busStop, pinSDA, pinSCL) {};


/*
  Initialize two wire bus and sensor with parameters stored by constructor.

  DESCRIPTION:
  The method sanitizes and stores input parameters to the class instance object,
  which determines the operation modus of the sensor.

  PARAMETERS:
  address - One of two possible 7 bit addresses of the sensor or state
            of the ADDR pin. If it is not some of metioned values, it fallbacks
            to default value.
            - Data type: non-negative integer
            - Default value: ADDRESS_LOW
            - Limited range: ADDRESS_LOW, ADDRESS_HIGH, HIGH, LOW

  mode - Measurement mode from possible listed ones.
         - Data type: non-negative integer
         - Default value: MODE_CONTINUOUS_HIGH
         - Limited range: MODE_CONTINUOUS_HIGH ~ MODE_ONETIME_LOW

  RETURN:
  Result code.
*/
uint8_t begin(uint8_t address = ADDRESS_LOW, uint8_t mode = MODE_CONTINUOUS_HIGH);


/*
  Activate sensor.

  DESCRIPTION:
  The method puts the sensor to the state waiting on measurement command.

  PARAMETERS: none

  RETURN:
  Result code.
*/
uint8_t powerOn();


/*
  Deactivate sensor.

  DESCRIPTION:
  The method puts the sensor to the non-active (sleep) state.

  PARAMETERS: none

  RETURN:
  Result code.
*/
uint8_t powerDown();


/*
  Reset sensor.

  DESCRIPTION:
  The method resets the illuminance data register of the sensor and removes
  previous measurement result.

  PARAMETERS: none

  RETURN:
  Result code.
*/
uint8_t reset();


/*
  Measure current light intensity in lux.

  DESCRIPTION:

  PARAMETERS: none

  RETURN:
  Current light intensity in 0 ~ 54612 lux.
*/
uint16_t measureLight();


//------------------------------------------------------------------------------
// Public setters - they usually return result code.
//------------------------------------------------------------------------------
uint8_t setAddress(uint8_t address);
uint8_t setMode(uint8_t mode);


//------------------------------------------------------------------------------
// Public getters
//------------------------------------------------------------------------------
inline uint8_t getMode() { return _mode; };  // Current measurement mode
inline uint8_t getLightMSB() { return _light >> 8; };  // Recently measured high byte
inline uint8_t getLightLSB() { return _light & 0xFF; }; // Recently measured low byte
inline uint16_t getLight() { return _light; };  // Recently measured light value


private:
//------------------------------------------------------------------------------
// Private constants
//------------------------------------------------------------------------------
enum Commands
{
  CMD_POWER_DOWN = 0xE3,  // No active state
  CMD_POWER_ON = 0xE5,  // Wating for measurement command
  CMD_RESET = 0xFE,  // Reset data register value
};


//------------------------------------------------------------------------------
// Private attributes
//------------------------------------------------------------------------------
uint8_t _mode;  // Current measurement mode of the sensor
uint16_t _light;  // Light intensity in lux

};

#endif

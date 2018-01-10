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
#define GBJ_BH1750FVI_VERSION "GBJ_BH1750FVI 1.0.0"

#if defined(__AVR__)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
  #include <inttypes.h>
  #include <Wire.h>
#elif defined(PARTICLE)
  #include <Particle.h>
#endif

#include "gbj_twowire.h"

// Possible addresses
#define GBJ_BH1750FVI_ADDRESS_L         0x23    // ADDR <= 0.3Vcc
#define GBJ_BH1750FVI_ADDRESS_H         0x5C    // ADDR >= 0.7Vcc
#define GBJ_BH1750FVI_ADDRESS_DEF GBJ_BH1750FVI_ADDRESS_L // Default address

// Function commands
#define GBJ_BH1750FVI_POWER_DOWN        0x00    // No active state
#define GBJ_BH1750FVI_POWER_ON          0x01    // Wating for measurment command
#define GBJ_BH1750FVI_RESET             0x07    // Reset data register value

// Measurement (mode) commands
#define GBJ_BH1750FVI_CONTINUOUS_HIGH   0x10    // 1lx/120ms
#define GBJ_BH1750FVI_CONTINUOUS_HIGH2  0x11    // 0.5lx/120ms
#define GBJ_BH1750FVI_CONTINUOUS_LOW    0x13    // 4lx/16ms
#define GBJ_BH1750FVI_ONETIME_HIGH      0x20    // 1lx/120ms
#define GBJ_BH1750FVI_ONETIME_HIGH2     0x21    // 0.5lx/120ms
#define GBJ_BH1750FVI_ONETIME_LOW       0x23    // 4lx/16ms

// Result codes
#define GBJ_BH1750FVI_ERR_MODE          255     // Bad measurement mode
#define GBJ_BH1750FVI_VALUE_BAD         0xFFFF  // Value for bad measurement mode
#define GBJ_BH1750FVI_MODE_DEF          GBJ_BH1750FVI_CONTINUOUS_HIGH // Default measurement mode


class gbj_bh1750fvi : public gbj_twowire
{
public:
//------------------------------------------------------------------------------
// Public methods
//------------------------------------------------------------------------------


/*
  Initialize two wire bus and sensor with parameters stored by constructor.

  DESCRIPTION:
  The method sanitizes and stores input parameters to the class instance object,
  which determines the operation modus of the sensor.

  PARAMETERS:
  address - One of two possible 7 bit addresses of the sensor or state
            of the ADDR pin.
            - Data type: non-negative integer
            - Default value: GBJ_BH1750FVI_ADDRESS_L
            - Limited range: GBJ_BH1750FVI_ADDRESS_L, GBJ_BH1750FVI_ADDRESS_H, HIGH, LOW

  mode - Measurement mode from possible listed ones.
         - Data type: non-negative integer
         - Default value: GBJ_BH1750FVI_CONTINUOUS_HIGH
         - Limited range: GBJ_BH1750FVI_CONTINUOUS_HIGH ~ GBJ_BH1750FVI_ONETIME_LOW

  busStop - Flag about releasing the bus after end of data transmission.
            - Data type: boolean
            - Default value: true
            - Limited range: true, false

  RETURN:
  Result code.
*/
  uint8_t begin( \
    uint8_t address = GBJ_BH1750FVI_ADDRESS_DEF, \
    uint8_t mode = GBJ_BH1750FVI_MODE_DEF, \
    bool busStop = true);


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
  uint8_t  getMode();         // Current measurement mode
  uint16_t getLight();        // Recently measured light value
  uint8_t  getLightMSB();     // Recently measured high byte
  uint8_t  getLightLSB();     // Recently measured low byte


private:
//------------------------------------------------------------------------------
// Private attributes
//------------------------------------------------------------------------------
  uint8_t  _mode;             // Current measurement mode of the sensor
  uint8_t  _lightMSB;         // Most significant byte of the raw light value
  uint8_t  _lightLSB;         // Least significant byte of the raw light value
  uint16_t _lightValue;       // Final light intensity in lux

};

#endif

/*
  NAME:
  gbj_bh1750

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
  GitHub: https://github.com/mrkaleArduinoLib/gbj_bh1750.git

  CREDITS:
  TimChristopher Laws
  https://github.com/claws/BH1750.git
 */
#ifndef GBJ_BH1750_H
#define GBJ_BH1750_H

#include "gbj_twowire.h"


class gbj_bh1750 : public gbj_twowire
{
public:
//------------------------------------------------------------------------------
// Public constants
//------------------------------------------------------------------------------
static const String VERSION;
enum Addresses
{
  ADDRESS_GND = 0x23,  // ADDR <= 0.3Vcc
  ADDRESS_FLOAT = ADDRESS_GND,
  ADDRESS_VCC = 0x5C,  // ADDR >= 0.7Vcc
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
gbj_bh1750(uint32_t clockSpeed = CLOCK_100KHZ, uint8_t pinSDA = 4, uint8_t pinSCL = 5) \
: gbj_twowire(clockSpeed, pinSDA, pinSCL) {};


/*
  Initialize two wire bus and sensor with parameters stored by constructor.

  DESCRIPTION:
  The method sanitizes and stores input parameters to the class instance object,
  which determines the operation modus of the sensor.
  - All measurement parameters are set to default or recommended values.

  PARAMETERS:
  address - One of two possible 7 bit addresses of the sensor or state
            of the ADDR pin. If it is not some of metioned values, it fallbacks
            to default value.
            - Data type: non-negative integer
            - Default value: ADDRESS_GND
            - Limited range: ADDRESS_GND, ADDRESS_VCC, HIGH, LOW

  mode - Measurement mode from possible listed ones.
         - Data type: non-negative integer
         - Default value: MODE_CONTINUOUS_HIGH
         - Limited range: MODE_CONTINUOUS_HIGH ~ MODE_ONETIME_LOW

  RETURN:
  Result code.
*/
uint8_t begin(uint8_t address = ADDRESS_GND, uint8_t mode = MODE_CONTINUOUS_HIGH);


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
uint8_t powerOff();


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
  Measure ambient light intensity in lux.

  DESCRIPTION:

  PARAMETERS: none

  RETURN:
  Result code.
*/
uint8_t measureLight();


/*
  Measure and return ambient light intensity in lux at particular accuracy.

  DESCRIPTION:

  PARAMETERS: none

  RETURN:
  Ambient light intensity in lux.
*/
inline float measureLightTyp() { return (measureLight() == gbj_bh1750::SUCCESS) ? getLightTyp() : 0.0; };
inline float measureLightMax() { return (measureLight() == gbj_bh1750::SUCCESS) ? getLightMax() : 0.0; };
inline float measureLightMin() { return (measureLight() == gbj_bh1750::SUCCESS) ? getLightMin() : 0.0; };


//------------------------------------------------------------------------------
// Public setters - they usually return result code.
//------------------------------------------------------------------------------
uint8_t setAddress(uint8_t address);
uint8_t setMode(uint8_t mode);
inline void setTimingTyp() { _status.flagMaxMeasurementTime = false; };
inline void setTimingMax() { _status.flagMaxMeasurementTime = true; };
uint8_t setResolutionVal(uint8_t mtreg);
inline uint8_t setResolutionMin() { return setResolutionVal(MTREG_MIN); };
inline uint8_t setResolutionTyp() { return setResolutionVal(MTREG_TYP); };
inline uint8_t setResolutionMax() { return setResolutionVal(MTREG_MAX); };


//------------------------------------------------------------------------------
// Public getters
//------------------------------------------------------------------------------
inline uint8_t getMode() { return _status.mode; };
inline bool getTimingTyp() { return !_status.flagMaxMeasurementTime; };
inline bool getTimingMax() { return _status.flagMaxMeasurementTime; };
inline uint16_t getMeasurementTime() { return _status.measurementTime; };
inline uint16_t getMeasurementTimeTyp() { return _status.measurementTimeTyp; };
inline uint16_t getMeasurementTimeMax() { return _status.measurementTimeMax; };
inline uint16_t getLightResult() { return _light.result; };  // Recent value of data register
inline float getLightMin() { return _light.minimal; };  // Recently measured light value at minimal accuracy
inline float getLightTyp() { return _light.typical; };  // Recently measured light value at typical accuracy
inline float getLightMax() { return _light.maximal; };  // Recently measured light value at maximal accuracy
inline float getSensitivityMin() { return 100.0 / _status.senseCoef / (float) ACCURACY_MIN; };  // lux/bitCount
inline float getSensitivityTyp() { return 100.0 / _status.senseCoef / (float) ACCURACY_TYP; };
inline float getSensitivityMax() { return 100.0 / _status.senseCoef / (float) ACCURACY_MAX; };
inline float getAccuracyMin() { return (float) ACCURACY_MIN / 100.0; };  // bitCount/lux
inline float getAccuracyTyp() { return (float) ACCURACY_TYP / 100.0; };
inline float getAccuracyMax() { return (float) ACCURACY_MAX / 100.0; };
inline float getResolutionMin() { return 1 / getSensitivityMin(); };  // bitCount/lux
inline float getResolutionTyp() { return 1 / getSensitivityTyp(); };
inline float getResolutionMax() { return 1 / getSensitivityMax(); };


private:
//------------------------------------------------------------------------------
// Private constants
//------------------------------------------------------------------------------
enum Commands
{
  CMD_POWER_DOWN = 0x00,  // No active state
  CMD_POWER_ON = 0x01,  // Wating for measurement command
  CMD_RESET = 0x07,  // Reset data register value
  CMD_MTIME_HIGH = 0x40,  // Change high bits of measurement time
  CMD_MTIME_LOW = 0x60,  // Change low bits of measurement time
};
enum Timing
{
  TIMING_HIGHRESMODE_TYP = 120,  // Typical conversion time at high resolution mode in milliseconds
  TIMING_HIGHRESMODE_MAX = 180,  // Maximal conversion time at high resolution mode in milliseconds
  TIMING_LOWRESMODE_TYP = 16,  // Typical conversion time at low resolution mode in milliseconds
  TIMING_LOWRESMODE_MAX = 24,  // Maximal conversion time at low resolution mode in milliseconds
};
enum MeasurementAccuracy  // In fixed float format with 2 fraction digits
{
  ACCURACY_TYP = 120,  // Typical measurement accuracy 1.2 count/lux
  ACCURACY_MIN = 96,  // Minimal measurement accuracy 0.96 count/lux
  ACCURACY_MAX = 144,  // Maximal measurement accuracy 1.44 count/lux
};
enum MeasurementTiming
{
  MTREG_TYP = 69,  // Typical value of measurement time register
  MTREG_MIN = 31,  // Minimal value of measurement time register
  MTREG_MAX = 254,  // Maximal value of measurement time register
};


//------------------------------------------------------------------------------
// Private attributes
//------------------------------------------------------------------------------
struct
{
  uint8_t mode;  // Current measurement mode of the sensor
  uint8_t mtreg;  // Current value of measurement time register
  float senseCoef;  // Sensitivity coeficient
  bool flagMaxMeasurementTime;
  uint16_t measurementTime;  // In milliseconds
  uint16_t measurementTimeTyp;  // In milliseconds
  uint16_t measurementTimeMax;  // In milliseconds
} _status;  // Initially set to default values
struct
{
  uint16_t result;  // Sensor output of measurement
  float typical;  // Light intensity in lux at typical measurement accuracy
  float minimal;  // Light intensity in lux at minimal measurement accuracy
  float maximal;  // Light intensity in lux at maximal measurement accuracy
} _light;


//------------------------------------------------------------------------------
// Private methods
//------------------------------------------------------------------------------
void calculateLight();
void setMeasurementTime();
float calculateSenseCoef();  // Counts per lux

};

#endif

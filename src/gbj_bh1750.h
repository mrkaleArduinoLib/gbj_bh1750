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
  it under the terms of the license GNU GPL v3
  http://www.gnu.org/licenses/gpl-3.0.html (related to original code) and MIT
  License (MIT) for added code.

  CREDENTIALS:
  Author: Libor Gabaj
  GitHub: https://github.com/mrkaleArduinoLib/gbj_bh1750.git
 */
#ifndef GBJ_BH1750_H
#define GBJ_BH1750_H

#include "gbj_twowire.h"

class gbj_bh1750 : public gbj_twowire
{
public:
  enum Addresses : uint8_t
  {
    // ADDR <= 0.3Vcc, connected to GND
    ADDRESS_GND = 0x23,
    // ADDR >= 0.7Vcc, connected to Vcc
    ADDRESS_VCC = 0x5C,
    ADDRESS_FLOAT = Addresses::ADDRESS_GND,
  };
  enum Modes : uint8_t
  {
    MODE_CONTINUOUS_HIGH = 0x10, // 1 lx / 120 ms
    MODE_CONTINUOUS_HIGH2 = 0x11, // 0.5 lx / 120 ms
    MODE_CONTINUOUS_LOW = 0x13, // 4 lx / 16 ms
    MODE_ONETIME_HIGH = 0x20, // 1 lx / 120 ms
    MODE_ONETIME_HIGH2 = 0x21, // 0.5 lx / 120 ms
    MODE_ONETIME_LOW = 0x23, // 4 lx / 16 ms
  };

  gbj_bh1750(ClockSpeeds clockSpeed = ClockSpeeds::CLOCK_100KHZ,
             uint8_t pinSDA = 4,
             uint8_t pinSCL = 5)
    : gbj_twowire(clockSpeed, pinSDA, pinSCL)
  {
  }

  /*
    Initialize two wire bus and sensor with parameters stored by constructor.

    DESCRIPTION:
    The method sanitizes and stores input parameters to the class instance
    object, which determines the operation modus of the sensor.
    - All measurement parameters are set to default or recommended values.

    PARAMETERS:
    address - One of two possible 7 bit addresses of the sensor or state of the
    ADDR pin. If it is not some of metioned values, it fallbacks to default
    value.
      - Data type: Addresses
      - Default value: ADDRESS_GND
      - Limited range: ADDRESS_GND, ADDRESS_VCC, HIGH, LOW

    mode - Measurement mode from possible listed ones.
      - Data type: Modes
      - Default value: MODE_CONTINUOUS_HIGH
      - Limited range: MODE_CONTINUOUS_HIGH ~ MODE_ONETIME_LOW

    RETURN: Result code
  */
  inline ResultCodes begin(Addresses address = Addresses::ADDRESS_GND,
                           Modes mode = Modes::MODE_CONTINUOUS_HIGH)
  {
    if (isError(gbj_twowire::begin()))
    {
      return getLastResult();
    }
    if (isError(setAddress(address)))
    {
      return getLastResult();
    }
    if (isError(powerOn()))
    {
      return getLastResult();
    }
    if (isError(setMode(mode)))
    {
      return getLastResult();
    }
    return getLastResult();
  }

  /*
    Activate sensor.

    DESCRIPTION:
    The method puts the sensor to the state waiting on measurement command.

    PARAMETERS: none

    RETURN: Result code
  */
  inline ResultCodes powerOn() { return busSend(CMD_POWER_ON); }

  /*
    Deactivate sensor.

    DESCRIPTION:
    The method puts the sensor to the non-active (sleep) state.

    PARAMETERS: none

    RETURN: Result code
  */
  inline ResultCodes powerOff() { return busSend(CMD_POWER_DOWN); }

  /*
    Reset sensor.

    DESCRIPTION:
    The method resets the illuminance data register of the sensor and removes
    previous measurement result.

    PARAMETERS: none

    RETURN: Result code
  */
  inline ResultCodes reset()
  {
    bool origBusStop = getBusStop();
    setBusRpte();
    if (isError(powerOn()))
    {
      return getLastResult();
    }
    if (isError(busSend(Commands::CMD_RESET)))
    {
      return getLastResult();
    }
    setBusStopFlag(origBusStop);
    if (isError(setMode(getMode())))
    {
      return getLastResult();
    }
    return getLastResult();
  }

  /*
    Measure ambient light intensity in lux.

    DESCRIPTION: none

    PARAMETERS: none

    RETURN: Result code
  */
  inline ResultCodes measureLight()
  {
    switch (getMode())
    {
      case Modes::MODE_ONETIME_LOW:
      case Modes::MODE_ONETIME_HIGH:
      case Modes::MODE_ONETIME_HIGH2:
        // Wake up the sensor
        if (isError(setMode(getMode())))
        {
          return getLastResult();
        }
        break;
      default:
        break;
    }
    uint8_t data[2];
    if (isError(busReceive(data, sizeof(data) / sizeof(data[0]))))
    {
      return getLastResult();
    }
    light_.result = (data[0] << 8) | data[1];
    calculateLight();
    return getLastResult();
  }

  /*
    Measure and return ambient light intensity in lux at particular accuracy.

    DESCRIPTION:
    If measurement fails, the method returns zero value.

    PARAMETERS: none

    RETURN: Ambient light intensity in lux
  */
  inline float measureLightTyp()
  {
    return isSuccess(measureLight()) ? getLightTyp() : 0.0;
  }
  inline float measureLightMax()
  {
    return isSuccess(measureLight()) ? getLightMax() : 0.0;
  }
  inline float measureLightMin()
  {
    return isSuccess(measureLight()) ? getLightMin() : 0.0;
  }

  // Setters
  ResultCodes setAddress(Addresses address);
  ResultCodes setMode(Modes mode);
  inline void setTimingTyp() { status_.flagMaxMeasurementTime = false; }
  inline void setTimingMax() { status_.flagMaxMeasurementTime = true; }
  inline ResultCodes setResolutionMin()
  {
    return setResolutionVal(MeasurementTiming::MTREG_MIN);
  }
  inline ResultCodes setResolutionTyp()
  {
    return setResolutionVal(MeasurementTiming::MTREG_TYP);
  }
  inline ResultCodes setResolutionMax()
  {
    return setResolutionVal(MeasurementTiming::MTREG_MAX);
  }

  // Getters
  inline Modes getMode() { return status_.mode; }
  inline bool getTimingTyp() { return !status_.flagMaxMeasurementTime; }
  inline bool getTimingMax() { return status_.flagMaxMeasurementTime; }
  inline uint16_t getMeasurementTime() { return status_.measurementTime; }
  inline uint16_t getMeasurementTimeTyp() { return status_.measurementTimeTyp; }
  inline uint16_t getMeasurementTimeMax() { return status_.measurementTimeMax; }
  // Recent value of data register
  inline uint16_t getLightResult() { return light_.result; }
  // Recently measured light at minimal accuracy, so at maximal sensitivity
  inline float getLightMin() { return light_.minimal; }
  // Recently measured light at typical accuracy, so at maximal sensitivity
  inline float getLightTyp() { return light_.typical; }
  // Recently measured light at maximal accuracy, so at maximal sensitivity
  inline float getLightMax() { return light_.maximal; }
  // Recently set sensitivity coefficient (lux/bitCount)
  inline float getSenseCoef() { return status_.senseCoef; }
  // lux/bitCount
  inline float getSensitivityMin()
  {
    return 100.0 / getSenseCoef() / static_cast<float>(ACCURACY_MAX);
  }
  inline float getSensitivityTyp()
  {
    return 100.0 / getSenseCoef() / static_cast<float>(ACCURACY_TYP);
  }
  inline float getSensitivityMax()
  {
    return 100.0 / getSenseCoef() / static_cast<float>(ACCURACY_MIN);
  }
  // bitCount/lux
  inline float getAccuracyMin()
  {
    return static_cast<float>(ACCURACY_MIN) / 100.0;
  }
  inline float getAccuracyTyp()
  {
    return static_cast<float>(ACCURACY_TYP) / 100.0;
  }
  inline float getAccuracyMax()
  {
    return static_cast<float>(ACCURACY_MAX) / 100.0;
  }
  // bitCount/lux
  inline float getResolutionMin() { return 1.0 / getSensitivityMin(); }
  inline float getResolutionTyp() { return 1.0 / getSensitivityTyp(); }
  inline float getResolutionMax() { return 1.0 / getSensitivityMax(); }

private:
  enum Commands : uint8_t
  {
    CMD_POWER_DOWN = 0x00, // No active state
    CMD_POWER_ON = 0x01, // Wating for measurement command
    CMD_RESET = 0x07, // Reset data register value
    CMD_MTIME_HIGH = 0x40, // Change high bits of measurement time
    CMD_MTIME_LOW = 0x60, // Change low bits of measurement time
  };
  enum Timing : uint8_t
  {
    // Typical conversion time at high resolution mode in milliseconds
    TIMING_HIGHRESMODE_TYP = 120,
    // Maximal conversion time at high resolution mode in milliseconds
    TIMING_HIGHRESMODE_MAX = 180,
    // Typical conversion time at low resolution mode in milliseconds
    TIMING_LOWRESMODE_TYP = 16,
    // Maximal conversion time at low resolution mode in milliseconds
    TIMING_LOWRESMODE_MAX = 24,
    // Safety percentage increase of a final conversion time
    TIMING_SAFETY_PERC = 5,
  };
  enum MeasurementTiming : uint8_t
  {
    MTREG_TYP = 69, // Typical value of measurement time register
    MTREG_MIN = 31, // Minimal value of measurement time register
    MTREG_MAX = 254, // Maximal value of measurement time register
  };
  // In fixed float format with 2 fraction digits
  enum MeasurementAccuracy : uint8_t
  {
    ACCURACY_TYP = 120, // Typical measurement accuracy 1.2 count/lux
    ACCURACY_MIN = 96, // Minimal measurement accuracy 0.96 count/lux
    ACCURACY_MAX = 144, // Maximal measurement accuracy 1.44 count/lux
  };
  // Initially set to default values
  struct Status
  {
    Modes mode; // Current measurement mode of the sensor
    MeasurementTiming mtreg; // Current value of measurement time register
    float senseCoef; // Sensitivity coeficient
    bool flagMaxMeasurementTime;
    uint16_t measurementTime; // In milliseconds
    uint16_t measurementTimeTyp; // In milliseconds
    uint16_t measurementTimeMax; // In milliseconds
  } status_;
  struct Light
  {
    uint16_t result; // Sensor output of measurement
    float typical; // Light intensity in lux at typical measurement accuracy
    float minimal; // Light intensity in lux at minimal measurement accuracy
    float maximal; // Light intensity in lux at maximal measurement accuracy
  } light_;
  inline void calculateLight()
  {
    light_.typical = static_cast<float>(light_.result) * getSensitivityTyp();
    light_.minimal = static_cast<float>(light_.result) * getSensitivityMin();
    light_.maximal = static_cast<float>(light_.result) * getSensitivityMax();
  }
  // Counts per lux
  inline float calculateSenseCoef()
  {
    status_.senseCoef = static_cast<float>(status_.mtreg) /
                        static_cast<float>(MeasurementTiming::MTREG_TYP);
    switch (getMode())
    {
      case Modes::MODE_CONTINUOUS_HIGH2:
      case Modes::MODE_ONETIME_HIGH2:
        status_.senseCoef *= 2.0;
        break;
      default:
        break;
    }
    return status_.senseCoef;
  }
  void setMeasurementTime();
  ResultCodes setResolutionVal(MeasurementTiming mtreg);
};

#endif

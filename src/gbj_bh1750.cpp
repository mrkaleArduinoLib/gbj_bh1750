#include "gbj_bh1750.h"

gbj_bh1750::ResultCodes gbj_bh1750::setAddress(Addresses address)
{
  switch (address)
  {
    case Addresses::ADDRESS_GND:
    case Addresses::ADDRESS_VCC:
      break;
    default:
      address = Addresses::ADDRESS_GND;
      break;
  }
  return registerAddress(address);
}

gbj_bh1750::ResultCodes gbj_bh1750::setResolutionVal(MeasurementTiming mtreg)
{
  switch (getMode())
  {
    // Set to default at low resolution mode
    case Modes::MODE_CONTINUOUS_LOW:
    case Modes::MODE_ONETIME_LOW:
      mtreg = MeasurementTiming::MTREG_TYP;
      break;
    default:
      mtreg = mtreg ? mtreg : MeasurementTiming::MTREG_TYP;
      mtreg = constrain(
        mtreg, MeasurementTiming::MTREG_MIN, MeasurementTiming::MTREG_MAX);
      break;
  }
  // Send to the bus at change only
  if (_status.mtreg != mtreg)
  {
    _status.mtreg = mtreg;
    bool origBusStop = getBusStop();
    // High 3 bits
    uint8_t mtregByte = Commands::CMD_MTIME_HIGH | (_status.mtreg >> 5);
    setBusRpte();
    if (isError(busSend(mtregByte)))
    {
      return getLastResult();
    }
    // Low 5 bits
    mtregByte = Commands::CMD_MTIME_LOW | (_status.mtreg & B11111);
    if (isError(busSend(mtregByte)))
    {
      return getLastResult();
    }
    setBusStopFlag(origBusStop);
  }
  // Set mode in the sensor
  if (isError(busSend(getMode())))
  {
    return getLastResult();
  }
  calculateSenseCoef();
  setMeasurementTime();
  setTimestampReceive();
  return getLastResult();
}

void gbj_bh1750::setMeasurementTime()
{
  uint8_t defaultMeasurementTimeTyp, defaultMeasurementTimeMax;
  switch (getMode())
  {
    case Modes::MODE_CONTINUOUS_LOW:
    case Modes::MODE_ONETIME_LOW:
      defaultMeasurementTimeTyp = Timing::TIMING_LOWRESMODE_TYP;
      defaultMeasurementTimeMax = Timing::TIMING_LOWRESMODE_MAX;
      break;
    default:
      defaultMeasurementTimeTyp = Timing::TIMING_HIGHRESMODE_TYP;
      defaultMeasurementTimeMax = Timing::TIMING_HIGHRESMODE_MAX;
      break;
  }
  _status.measurementTimeTyp = _status.senseCoef * defaultMeasurementTimeTyp;
  _status.measurementTimeMax = _status.senseCoef * defaultMeasurementTimeMax;
  _status.measurementTime =
    getTimingMax() ? _status.measurementTimeMax : _status.measurementTimeTyp;
  _status.measurementTime *= 1.0 + float(Timing::TIMING_SAFETY_PERC) / 100.0;
  // Limit minimal value of measurement time to typical value
  _status.measurementTime =
    max(_status.measurementTime, defaultMeasurementTimeTyp);
  gbj_twowire::setDelayReceive(_status.measurementTime);
}

gbj_bh1750::ResultCodes gbj_bh1750::setMode(Modes mode)
{
  switch (mode)
  {
    case Modes::MODE_CONTINUOUS_LOW:
    case Modes::MODE_CONTINUOUS_HIGH:
    case Modes::MODE_CONTINUOUS_HIGH2:
    case Modes::MODE_ONETIME_LOW:
    case Modes::MODE_ONETIME_HIGH:
    case Modes::MODE_ONETIME_HIGH2:
      break;
    default:
      mode = Modes::MODE_CONTINUOUS_HIGH;
      break;
  }
  _status.mode = mode;
  switch (getMode())
  {
    case Modes::MODE_CONTINUOUS_LOW:
    case Modes::MODE_ONETIME_LOW:
      if (setResolutionTyp())
        return getLastResult();
      break;
    default:
      if (setResolutionVal(_status.mtreg))
        return getLastResult();
      break;
  }
  return getLastResult();
}

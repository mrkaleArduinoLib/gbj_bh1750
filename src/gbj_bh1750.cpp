#include "gbj_bh1750.h"
const String gbj_bh1750::VERSION = "GBJ_BH1750 1.0.0";


uint8_t gbj_bh1750::begin(uint8_t address, uint8_t mode)
{
  if (gbj_twowire::begin()) return getLastResult();
  if (setAddress(address)) return getLastResult();
  if (powerOn()) return getLastResult();
  if (setMode(mode)) return getLastResult();
  return getLastResult();
}


uint8_t gbj_bh1750::powerOn()
{
  return busSend(CMD_POWER_ON);
}


uint8_t gbj_bh1750::powerOff()
{
  return busSend(CMD_POWER_DOWN);
}


uint8_t gbj_bh1750::reset()
{
  if (powerOn()) return getLastResult();
  return busSend(CMD_RESET);
}


void gbj_bh1750::calculateLight()
{
  _light.typical = (float)_light.result * getSensitivityTyp();
  _light.minimal = (float)_light.result * getSensitivityMin();
  _light.maximal = (float)_light.result * getSensitivityMax();
}


float gbj_bh1750::calculateSenseCoef()
{
  _status.senseCoef = (float) _status.mtreg / (float) MTREG_TYP;
  switch (getMode())
  {
    case MODE_CONTINUOUS_HIGH2:
    case MODE_ONETIME_HIGH2:
      _status.senseCoef *= 2.0;
      break;
  }
  return _status.senseCoef;
}


uint8_t gbj_bh1750::measureLight()
{
  switch (getMode())
  {
    case MODE_ONETIME_LOW:
    case MODE_ONETIME_HIGH:
    case MODE_ONETIME_HIGH2:
      if (setMode(getMode())) return getLastResult();  // Wake up the sensor
      break;
  }
  waitTimestampReceive();
  uint8_t data[2];
  if (busReceive(data, sizeof(data) / sizeof(data[0]))) return getLastResult();
  _light.result = (data[0] << 8) | data[1];
  calculateLight();
  return getLastResult();
}


//-------------------------------------------------------------------------
// Setters
//-------------------------------------------------------------------------
uint8_t gbj_bh1750::setAddress(uint8_t address)
{
  switch (address)
  {
    case LOW:
      address = ADDRESS_LOW;
      break;
    case HIGH:
      address = ADDRESS_HIGH;
      break;
    case ADDRESS_LOW:
    case ADDRESS_HIGH:
      break;
    default:
      address = ADDRESS_LOW;
      break;
  }
  return gbj_twowire::setAddress(address);
}


uint8_t gbj_bh1750::setResolutionVal(uint8_t mtreg)
{
  switch (getMode())
  {
    // Set to default at low resolution mode
    case MODE_CONTINUOUS_LOW:
    case MODE_ONETIME_LOW:
      mtreg = MTREG_TYP;
      break;
    default:
      if (mtreg == 0) mtreg = MTREG_TYP;
      mtreg = constrain(mtreg, MTREG_MIN, MTREG_MAX);
      break;
  }
  // Send to the buse at change only
  if (_status.mtreg != mtreg)
  {
    _status.mtreg = mtreg;
    uint8_t mtregByte = CMD_MTIME_HIGH | (_status.mtreg >> 5); // High 3 bits
    if (busSend(mtregByte)) return getLastResult();
    mtregByte = CMD_MTIME_LOW | (_status.mtreg & B11111); // Low 5 bits
    if (busSend(mtregByte)) return getLastResult();
  }
  if (busSend(getMode())) return getLastResult();  // Set mode in the sensor
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
    case MODE_CONTINUOUS_LOW:
    case MODE_ONETIME_LOW:
      defaultMeasurementTimeTyp = TIMING_LOWRESMODE_TYP;
      defaultMeasurementTimeMax = TIMING_LOWRESMODE_MAX;
      break;
    default:
      defaultMeasurementTimeTyp = TIMING_HIGHRESMODE_TYP;
      defaultMeasurementTimeMax = TIMING_HIGHRESMODE_MAX;
      break;
  }
  _status.measurementTimeTyp = _status.senseCoef * defaultMeasurementTimeTyp;
  _status.measurementTimeMax = _status.senseCoef * defaultMeasurementTimeMax;
  _status.measurementTime = getTimingMax() ? _status.measurementTimeMax : _status.measurementTimeTyp;
  // Limit minimal value of measurement time to typical value
  _status.measurementTime = max(_status.measurementTime, defaultMeasurementTimeTyp);
  gbj_twowire::setDelayReceive(_status.measurementTime);
}


uint8_t gbj_bh1750::setMode(uint8_t mode)
{
  switch (mode)
  {
    case MODE_CONTINUOUS_HIGH:
    case MODE_CONTINUOUS_HIGH2:
    case MODE_ONETIME_HIGH:
    case MODE_ONETIME_HIGH2:
    case MODE_CONTINUOUS_LOW:
    case MODE_ONETIME_LOW:
      break;
    default:
      mode = MODE_CONTINUOUS_HIGH;
      break;
  }
  _status.mode = mode;
  switch (getMode())
  {
    case MODE_CONTINUOUS_LOW:
    case MODE_ONETIME_LOW:
      if (setResolutionTyp()) return getLastResult();
      break;
    default:
      if (setResolutionVal(_status.mtreg)) return getLastResult();
      break;
  }
  return getLastResult();
}

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
  _status.senseCoef = (float) _status.mtreg / (float) MTREG_DEF;
  switch (getMode())
  {
    case MODE_CONTINUOUS_HIGH2:
    case MODE_ONETIME_HIGH2:
      _status.senseCoef *= 2.0;
      break;
  }
  return _status.senseCoef;
}


// Measure light intensity
uint8_t gbj_bh1750::measureLight()
{
  if (busReceive((uint8_t *) &_light.result, sizeof(_light.result))) return getLastResult();
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


uint8_t gbj_bh1750::setSensitivityVal(uint8_t mtreg)
{
  switch (getMode())
  {
    // Set to default at low resolution mode
    case MODE_CONTINUOUS_LOW:
    case MODE_ONETIME_LOW:
      mtreg = MTREG_DEF;
      break;
    default:
      mtreg = constrain(mtreg, MTREG_MIN, MTREG_MAX);
      break;
  }
  _status.mtreg = mtreg;
  calculateSenseCoef();
  setDelaySend();
  uint8_t mtregByte = CMD_MTIME_HIGH | (_status.mtreg >> 5); // High 3 bits
  if (busSend(mtregByte)) return getLastResult();
  mtregByte = CMD_MTIME_LOW | (_status.mtreg & B11111); // Low 5 bits
  return busSend(mtregByte);
}


void gbj_bh1750::setDelaySend()
{
  uint8_t defaultConversionTimeTyp, defaultConversionTimeMax;
  switch (getMode())
  {
    case MODE_CONTINUOUS_LOW:
    case MODE_ONETIME_LOW:
      defaultConversionTimeTyp = TIMING_LOWRESMODE_TYP;
      defaultConversionTimeMax = TIMING_LOWRESMODE_MAX;
      break;
    default:
      defaultConversionTimeTyp = TIMING_HIGHRESMODE_TYP;
      defaultConversionTimeMax = TIMING_HIGHRESMODE_MAX;
      break;
  }
  _status.conversionTimeTyp = _status.senseCoef * defaultConversionTimeTyp;
  _status.conversionTimeMax = _status.senseCoef * defaultConversionTimeMax;
  _status.conversionTime = getTimingMax() ? _status.conversionTimeMax : _status.conversionTimeTyp;
  gbj_twowire::setDelaySend(_status.conversionTime);
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
      if (setSensitivityTyp()) return getLastResult();
      break;
    default:
      setDelaySend();
      break;
  }
  return busSend(getMode());
}

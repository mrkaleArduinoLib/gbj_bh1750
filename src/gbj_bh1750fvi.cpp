#include "gbj_bh1750fvi.h"


uint8_t gbj_bh1750fvi::begin(uint8_t address, uint8_t mode, bool busStop)
{
  setBusStop(busStop);
  if (setAddress(address)) return getLastResult();
  if (powerOn()) return getLastResult();
  if (setMode(mode)) return getLastResult();
  return getLastResult();
}


uint8_t gbj_bh1750fvi::powerOn()
{
  return busSend(GBJ_BH1750FVI_POWER_ON);
}


uint8_t gbj_bh1750fvi::powerDown()
{
  return busSend(GBJ_BH1750FVI_POWER_DOWN);
}


uint8_t gbj_bh1750fvi::reset()
{
  if (powerOn()) return getLastResult();
  return busSend(GBJ_BH1750FVI_RESET);
}


// Measure light intensity
uint16_t gbj_bh1750fvi::measureLight()
{
  // Send one-time mode and wait for the measurement
  switch (getMode())
  {
    case GBJ_BH1750FVI_ONETIME_HIGH:
    case GBJ_BH1750FVI_ONETIME_HIGH2:
      if (busSend(getMode())) return getLastResult();
      wait(180);
      break;
    case GBJ_BH1750FVI_ONETIME_LOW:
      if (busSend(getMode())) return getLastResult();
      wait(24);
      break;
  }
  // Read measured value
  uint8_t data[2];
  if (busReceive(data, sizeof(data)/sizeof(data[0])))
  {
    // Erroneous measurement result
    _lightMSB = GBJ_BH1750FVI_VALUE_BAD >> 8;
    _lightLSB = GBJ_BH1750FVI_VALUE_BAD & 0xFF;
    _lightValue = GBJ_BH1750FVI_VALUE_BAD;
  }
  else
  {
    _lightMSB = data[0];        // High byte
    _lightLSB = data[1];        // Low byte
    _lightValue = _lightMSB << 8;
    _lightValue |= _lightLSB;
    _lightValue /= 1.2;
  }
  return _lightValue;
}


//-------------------------------------------------------------------------
// Setters
//-------------------------------------------------------------------------
uint8_t gbj_bh1750fvi::setAddress(uint8_t address)
{
  switch (address)
  {
    case HIGH:
      address = GBJ_BH1750FVI_ADDRESS_H;
      break;

    case LOW:
      address = GBJ_BH1750FVI_ADDRESS_L;
      break;
  }
  return gbj_twowire::setAddress(address);
}


uint8_t gbj_bh1750fvi::setMode(uint8_t mode)
{
  initLastResult();
  // Sanitize mode
  switch (mode)
  {
    case GBJ_BH1750FVI_CONTINUOUS_HIGH:
    case GBJ_BH1750FVI_CONTINUOUS_HIGH2:
    case GBJ_BH1750FVI_ONETIME_HIGH:
    case GBJ_BH1750FVI_ONETIME_HIGH2:
    case GBJ_BH1750FVI_CONTINUOUS_LOW:
    case GBJ_BH1750FVI_ONETIME_LOW:
      break;

    default:
      return setLastResult(GBJ_BH1750FVI_ERR_MODE);
      break;
  }
  // No mode change
  if (mode == getMode()) return getLastResult();
  // Set changed mode
  _mode = mode;
  // Send continues mode and wait for the first measurement
  switch (getMode())
  {
    case GBJ_BH1750FVI_CONTINUOUS_HIGH:
    case GBJ_BH1750FVI_CONTINUOUS_HIGH2:
      if (busSend(getMode())) return getLastResult();
      wait(180);
      break;
    case GBJ_BH1750FVI_CONTINUOUS_LOW:
      if (busSend(getMode())) return getLastResult();
      wait(24);
      break;
  }
  return getLastResult();
}


//-------------------------------------------------------------------------
// Getters
//-------------------------------------------------------------------------
uint8_t  gbj_bh1750fvi::getMode()     { return _mode; }
uint16_t gbj_bh1750fvi::getLight()    { return _lightValue; }
uint8_t  gbj_bh1750fvi::getLightMSB() { return _lightMSB; }
uint8_t  gbj_bh1750fvi::getLightLSB() { return _lightLSB; }

#include "gbj_bh1750fvi.h"
const String gbj_bh1750fvi::VERSION = "GBJ_BH1750FVI 1.0.0";


uint8_t gbj_bh1750fvi::begin(uint8_t address, uint8_t mode)
{
  if (gbj_twowire::begin()) return getLastResult();
  if (setAddress(address)) return getLastResult();
  if (powerOn()) return getLastResult();
  if (setMode(mode)) return getLastResult();
  return getLastResult();
}


uint8_t gbj_bh1750fvi::powerOn()
{
  return busSend(CMD_POWER_ON);
}


uint8_t gbj_bh1750fvi::powerDown()
{
  return busSend(CMD_POWER_DOWN);
}


uint8_t gbj_bh1750fvi::reset()
{
  if (powerOn()) return getLastResult();
  return busSend(CMD_RESET);
}


// Measure light intensity
uint16_t gbj_bh1750fvi::measureLight()
{
  // Send one-time mode and wait for the measurement
  switch (getMode())
  {
    case MODE_ONETIME_HIGH:
    case MODE_ONETIME_HIGH2:
      if (busSend(getMode())) return getLastResult();
      wait(180);
      break;
    case MODE_ONETIME_LOW:
      if (busSend(getMode())) return getLastResult();
      wait(24);
      break;
  }
  // Read measured value
  uint8_t data[2];
  if (busReceive(data, sizeof(data)/sizeof(data[0])))
  {
    // Erroneous measurement result
    _light = 0xFFFF;
  }
  else
  {
    _light = data[0] << 8;  // High byte
    _light |= data[1];  // Low byte
    _light /= 1.2;
  }
  return _light;
}


//-------------------------------------------------------------------------
// Setters
//-------------------------------------------------------------------------
uint8_t gbj_bh1750fvi::setAddress(uint8_t address)
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


uint8_t gbj_bh1750fvi::setMode(uint8_t mode)
{
  // Sanitize mode
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
  // No mode change
  if (mode == getMode()) return getLastResult();
  _mode = mode;
  // Send continues mode and wait for the first measurement
  initLastResult();
  switch (getMode())
  {
    case MODE_CONTINUOUS_HIGH:
    case MODE_CONTINUOUS_HIGH2:
      if (busSend(getMode())) return getLastResult();
      wait(180);
      break;
    case MODE_CONTINUOUS_LOW:
      if (busSend(getMode())) return getLastResult();
      wait(24);
      break;
  }
  return getLastResult();
}

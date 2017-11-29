#include <gbj_bh1750fvi.h>


uint8_t gbj_bh1750fvi::begin(uint8_t address, uint8_t mode, bool busStop)
{
  initLastResult();
  if (setAddress(address)) return getLastResult();
  if (setMode(mode)) return getLastResult();
  setBusStop(busStop);
  return getLastResult();
}


// Measure light intensity
uint16_t gbj_bh1750fvi::measureLight()
{
  uint8_t byteCount = 2;
  _lightValue = 0xFFFF; // Wrong light values
  _lightMSB = _lightLSB = 0xFF;
  initBus();
  Wire.beginTransmission(getAddress());
  if (Wire.requestFrom(getAddress(), byteCount, (uint8_t) getBusStop()) > 0 \
  && Wire.available() >= byteCount)
  {
    _lightMSB = Wire.read();        // Read high byte first
    _lightLSB = Wire.read();        // Read low byte
    _lightValue = ((_lightMSB << 8) | _lightLSB) / 6 * 5; // Division by 1.2
  }
  setLastResult(Wire.endTransmission(getBusStop()));
  return _lightValue;
}


//-------------------------------------------------------------------------
// Setters
//-------------------------------------------------------------------------
uint8_t gbj_bh1750fvi::setAddress(uint8_t address)
{
  return gbj_twowire::setAddress(sanitizeAddress(address));
}


uint8_t gbj_bh1750fvi::setMode(uint8_t mode)
{
  initLastResult();
  _mode = sanitizeMode(mode);
  // Invalid mode
  if (getMode() == GBJ_BH1750FVI_MODE_BAD)
  {
    setLastResult(GBJ_BH1750FVI_ERR_MODE);
    return getLastResult();
  }
  // No change in valid mode
  if (mode == getMode())
  {
    return getLastResult();
  }
  // Set changed mode
  initBus();
  Wire.beginTransmission(getAddress());
  Wire.write(getMode());
  if (setLastResult(Wire.endTransmission(getBusStop()))) return getLastResult();
  wait(10); // Wait for waking up
  return getLastResult();
}


//-------------------------------------------------------------------------
// Getters
//-------------------------------------------------------------------------
uint8_t  gbj_bh1750fvi::getMode()     { return _mode; }
uint16_t gbj_bh1750fvi::getLight()    { return _lightValue; }
uint8_t  gbj_bh1750fvi::getLightMSB() { return _lightMSB; }
uint8_t  gbj_bh1750fvi::getLightLSB() { return _lightLSB; }


//-------------------------------------------------------------------------
// Private methods
//-------------------------------------------------------------------------
uint8_t gbj_bh1750fvi::sanitizeAddress(uint8_t address)
{
  switch (address)
  {
    case HIGH:
      return GBJ_BH1750FVI_ADDRESS_H;
      break;

    case LOW:
      return GBJ_BH1750FVI_ADDRESS_L;
      break;

    case GBJ_BH1750FVI_ADDRESS_L:
    case GBJ_BH1750FVI_ADDRESS_H:
      return address;
      break;

    default:
      return GBJ_TWOWIRE_ADDRESS_BAD;
      break;
  }
}


uint8_t gbj_bh1750fvi::sanitizeMode(uint8_t mode)
{
  switch (mode)
  {
    case GBJ_BH1750FVI_CONTINUOUS_HIGH:
    case GBJ_BH1750FVI_CONTINUOUS_HIGH2:
    case GBJ_BH1750FVI_CONTINUOUS_LOW:
    case GBJ_BH1750FVI_ONETIME_HIGH:
    case GBJ_BH1750FVI_ONETIME_HIGH2:
    case GBJ_BH1750FVI_ONETIME_LOW:
      return mode;
      break;

    default:
      return GBJ_BH1750FVI_MODE_BAD;
      break;
  }
}

/*
  NAME:
  Typical communication with BH1750 sensor for a protocol decoder.

  DESCRIPTION:
  The sketch executes communication with the sensor in order to expose as much
  as possible from its functionality for testing and presenting protocol
  decoders, e.g., for logic analytical software sigrok.
  - Connect sensor's pins to microcontroller's I2C bus or I2C default pins
    as described in README.md for used platform accordingly.
  - Leave ADDR pin floating or connected to GND in order to use default address.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_bh1750.h"

gbj_bh1750 sensor = gbj_bh1750();
// gbj_bh1750 sensor = gbj_bh1750(sensor.CLOCK_100KHZ, true, D2, D1);
// gbj_bh1750 sensor = gbj_bh1750(sensor.CLOCK_400KHZ);

void errorHandler(String location)
{
  if (sensor.isSuccess())
  {
    return;
  }
  Serial.print(location);
  Serial.print(" - Error: ");
  Serial.print(sensor.getLastResult());
  Serial.print(" - ");
  switch (sensor.getLastResult())
  {
    // General
    case sensor.ERROR_ADDRESS:
      Serial.println("ERROR_ADDRESS");
      break;

    case sensor.ERROR_PINS:
      Serial.println("ERROR_PINS");
      break;

      // Arduino, Esspressif specific
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
    case sensor.ERROR_BUFFER:
      Serial.println("ERROR_BUFFER");
      break;

    case sensor.ERROR_NACK_DATA:
      Serial.println("ERROR_NACK_DATA");
      break;

    case sensor.ERROR_NACK_OTHER:
      Serial.println("ERROR_NACK_OTHER");
      break;

      // Particle specific
#elif defined(PARTICLE)
    case sensor.ERROR_BUSY:
      Serial.println("ERROR_BUSY");
      break;

    case sensor.ERROR_END:
      Serial.println("ERROR_END");
      break;

    case sensor.ERROR_TRANSFER:
      Serial.println("ERROR_TRANSFER");
      break;

    case sensor.ERROR_TIMEOUT:
      Serial.println("ERROR_TIMEOUT");
      break;
#endif

    default:
      Serial.println("Uknown error");
      break;
  }
}

void setup()
{
  Serial.begin(9600);
  Serial.println("---");
  if (sensor.isError(
        sensor.begin(sensor.ADDRESS_FLOAT, sensor.MODE_CONTINUOUS_HIGH2)))
  {
    errorHandler("Begin");
    return;
  }
  if (sensor.reset())
  {
    errorHandler("Reset");
    return;
  }
  if (sensor.setResolutionMax())
  {
    errorHandler("Resolution");
    return;
  }
  Serial.print(sensor.getSensitivityTyp());
  Serial.println(" lux/bitCount");
  if (sensor.measureLight())
  {
    errorHandler("Measure");
    return;
  }
  Serial.print(sensor.getLightTyp());
  Serial.println(" lux");
}

void loop() {}

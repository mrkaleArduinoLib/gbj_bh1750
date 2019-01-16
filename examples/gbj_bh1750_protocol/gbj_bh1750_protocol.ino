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
#define SKETCH "GBJ_BH1750_PROTOCOL 1.0.0"

#include "gbj_bh1750.h"

gbj_bh1750 Sensor = gbj_bh1750();
// gbj_bh1750 Sensor = gbj_bh1750(gbj_bh1750::CLOCK_100KHZ, true, D2, D1);
// gbj_bh1750 Sensor = gbj_bh1750(gbj_bh1750::CLOCK_400KHZ);


void errorHandler(String location)
{
  if (Sensor.isSuccess()) return;
  Serial.print(location);
  Serial.print(" - Error: ");
  Serial.print(Sensor.getLastResult());
  Serial.print(" - ");
  switch (Sensor.getLastResult())
  {
    // General
    case gbj_bh1750::ERROR_ADDRESS:
      Serial.println("ERROR_ADDRESS");
      break;

    case gbj_bh1750::ERROR_PINS:
      Serial.println("ERROR_PINS");
      break;

    // Arduino, Esspressif specific
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
    case gbj_bh1750::ERROR_BUFFER:
      Serial.println("ERROR_BUFFER");
      break;

    case gbj_bh1750::ERROR_NACK_DATA:
      Serial.println("ERROR_NACK_DATA");
      break;

    case gbj_bh1750::ERROR_NACK_OTHER:
      Serial.println("ERROR_NACK_OTHER");
      break;

    // Particle specific
#elif defined(PARTICLE)
    case gbj_bh1750::ERROR_BUSY:
      Serial.println("ERROR_BUSY");
      break;

    case gbj_bh1750::ERROR_END:
      Serial.println("ERROR_END");
      break;

    case gbj_bh1750::ERROR_TRANSFER:
      Serial.println("ERROR_TRANSFER");
      break;

    case gbj_bh1750::ERROR_TIMEOUT:
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
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(gbj_twowire::VERSION);
  Serial.println(gbj_bh1750::VERSION);
  Serial.println("---");

  if (Sensor.begin(gbj_bh1750::ADDRESS_FLOAT, gbj_bh1750::MODE_CONTINUOUS_HIGH2))
  {
    errorHandler("Begin");
    return;
  }
  if (Sensor.reset())
  {
    errorHandler("Reset");
    return;
  }
  if (Sensor.setResolutionMax())
  {
    errorHandler("Resolution");
    return;
  }
  Serial.print(Sensor.getSensitivityTyp());
  Serial.println(" lux/bitCount");
  if (Sensor.measureLight())
  {
    errorHandler("Measure");
    return;
  }
  Serial.print(Sensor.getLightTyp());
  Serial.println(" lux");
  Serial.println("END");
}


void loop() {}

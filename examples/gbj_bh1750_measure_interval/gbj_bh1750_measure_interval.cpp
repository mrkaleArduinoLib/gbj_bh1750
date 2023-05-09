/*
  NAME:
  Usage of gbjBH1750 library for ambient light measurement as an confidential
  interval.

  DESCRIPTION:
  The sketch measures ambient light intensity with one determined BH1750 sensor.
  - Connect sensor's pins to microcontroller's I2C bus or I2C default pins
    as described in README.md for used platform accordingly.
  - Leave ADDR pin floating or connected to GND in order to use default address.
  - Confidential interval of ambient light is range between measurement at
  minimal and maximal measurement accuracy, while within it there is measurement
  at typical accuracy.
  - Change measurement mode for various measurement sensitivity.
  - The sketch display the light result as well. It is a raw binary value from
    the sensor's data register.
  - The light result in hexadecimal form is useful for observing it in a logic
    analyser or in an oscilloscope.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_bh1750.h"

// Time in miliseconds between measurements
const unsigned int PERIOD_MEASURE = 3000;

gbj_bh1750 sensor = gbj_bh1750();
// gbj_bh1750 sensor = gbj_bh1750(sensor.CLOCK_100KHZ, D2, D1);
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

String getModeName()
{
  String measurementType;
  switch (sensor.getMode())
  {
    case sensor.MODE_CONTINUOUS_HIGH:
      measurementType = "Continuous High";
      break;
    case sensor.MODE_CONTINUOUS_HIGH2:
      measurementType = "Continuous High Double";
      break;
    case sensor.MODE_CONTINUOUS_LOW:
      measurementType = "Continuous Low";
      break;
    case sensor.MODE_ONETIME_HIGH:
      measurementType = "Onetime High";
      break;
    case sensor.MODE_ONETIME_HIGH2:
      measurementType = "Onetime High Double";
      break;
    case sensor.MODE_ONETIME_LOW:
      measurementType = "Onetime Low";
      break;
  }
  return measurementType;
}

void setup()
{
  Serial.begin(9600);
  if (sensor.isError(
        sensor.begin(sensor.ADDRESS_FLOAT, sensor.MODE_CONTINUOUS_HIGH)))
  {
    errorHandler("Begin");
    return;
  }
  Serial.println("---Config---");
  sensor.setResolutionMin();
  Serial.println("Resolution: Minimal");
  Serial.println("Mode: " + getModeName());
  Serial.println("SenseCoef: " + String(sensor.getSenseCoef()));
  Serial.println("---Params---");
  Serial.println("Accuracy (min/typ/max): " + String(sensor.getAccuracyMin()) +
                 "/" + String(sensor.getAccuracyTyp()) + "/" +
                 String(sensor.getAccuracyMax()) + " bitCount/lux");
  Serial.println(
    "Sensitivity (min/typ/max): " + String(sensor.getSensitivityMin()) + "/" +
    String(sensor.getSensitivityTyp()) + "/" +
    String(sensor.getSensitivityMax()) + " lux/bitCount");
  Serial.println(
    "Resolution (min/typ/max): " + String(sensor.getResolutionMin()) + "/" +
    String(sensor.getResolutionTyp()) + "/" +
    String(sensor.getResolutionMax()) + " bitCount/lux");
  Serial.println(
    "Measurement time (min/typ/max): " + String(sensor.getMeasurementTime()) +
    "/" + String(sensor.getMeasurementTimeTyp()) + "/" +
    String(sensor.getMeasurementTimeMax()) + " ms");
  Serial.println("---Measurement---");
  Serial.println("Result / Light in lux (Min, Typ, Max)");
}

void loop()
{
  if (sensor.isError())
  {
    return;
  }
  if (sensor.isError(sensor.measureLight()))
  {
    errorHandler("Measurement");
  }
  Serial.println((sensor.getLightResult() > 0x0F ? "0x" : "0x0") +
                 String(sensor.getLightResult(), HEX) + " / " +
                 String(sensor.getLightMin()) + " < " +
                 String(sensor.getLightTyp()) + " < " +
                 String(sensor.getLightMax()));
  delay(PERIOD_MEASURE);
}

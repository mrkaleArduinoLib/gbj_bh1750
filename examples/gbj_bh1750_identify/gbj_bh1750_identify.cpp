/*
  NAME:
  Fundamental parameters of BH1750FVI sensor using gbjBH1750 library.

  DESCRIPTION:
  The sketch display measurement and timing parameters of the ambient light
  BH1750FVI sensor for all continuous measurement modes, all measurement times
  (all sensitivities), and for entire accuracy interval (minimal, typical,
  maximal).
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
// gbj_bh1750 sensor = gbj_bh1750(sensor.CLOCK_100KHZ, D2, D1);
// gbj_bh1750 sensor = gbj_bh1750(sensor.CLOCK_400KHZ);

void errorHandler(String location)
{
  if (sensor.isSuccess())
    return;
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
  Serial.println("---");
  if (sensor.isError(
        sensor.begin(sensor.ADDRESS_FLOAT, sensor.MODE_CONTINUOUS_LOW)))
  {
    errorHandler("Begin");
    return;
  }
  Serial.println("Clock: " + String(sensor.getBusClock() / 1000) + " kHz");
  Serial.println("Address: 0x" + String(sensor.getAddress(), HEX));
  Serial.println("Accuracy (min/typ/max): " + String(sensor.getAccuracyMin()) +
                 "/" + String(sensor.getAccuracyTyp()) + "/" +
                 String(sensor.getAccuracyMax()) + " bitCount/lux");
  Serial.println("---");
  // Low mode
  Serial.println("Mode: " + getModeName());
  Serial.println("SenseCoef: " + String(sensor.getSenseCoef()));
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
  Serial.println("---");

  // High mode - typical sensitivity
  sensor.setMode(sensor.MODE_CONTINUOUS_HIGH);
  sensor.setResolutionTyp();
  Serial.println("Mode: " + getModeName() + " - Typical Sensitivity");
  Serial.println("SenseCoef: " + String(sensor.getSenseCoef()));
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
  Serial.println("---");

  // High mode - minimal sensitivity
  sensor.setMode(sensor.MODE_CONTINUOUS_HIGH);
  sensor.setResolutionMin();
  Serial.println("Mode: " + getModeName() + " - Minimal Sensitivity");
  Serial.println("SenseCoef: " + String(sensor.getSenseCoef()));
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
  Serial.println("---");

  // High mode - maximal sensitivity
  sensor.setMode(sensor.MODE_CONTINUOUS_HIGH);
  sensor.setResolutionMax();
  Serial.println("Mode: " + getModeName() + " - Maximal Sensitivity");
  Serial.println("SenseCoef: " + String(sensor.getSenseCoef()));
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
  Serial.println("---");

  // Double high mode - typical sensitivity
  sensor.setMode(sensor.MODE_CONTINUOUS_HIGH2);
  sensor.setResolutionTyp();
  Serial.println("Mode: " + getModeName() + " - Typical Sensitivity");
  Serial.println("SenseCoef: " + String(sensor.getSenseCoef()));
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
  Serial.println("---");

  // Double high mode - minimal sensitivity
  sensor.setMode(sensor.MODE_CONTINUOUS_HIGH2);
  sensor.setResolutionMin();
  Serial.println("Mode: " + getModeName() + " - Minimal Sensitivity");
  Serial.println("SenseCoef: " + String(sensor.getSenseCoef()));
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
  Serial.println("---");

  // Double high mode - maximal sensitivity
  sensor.setMode(sensor.MODE_CONTINUOUS_HIGH2);
  sensor.setResolutionMax();
  Serial.println("Mode: " + getModeName() + " - Maximal Sensitivity");
  Serial.println("SenseCoef: " + String(sensor.getSenseCoef()));
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
  Serial.println("---");
}

void loop() {}

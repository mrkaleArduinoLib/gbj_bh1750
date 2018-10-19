/*
  NAME:
  Basic usage of gbj_bh1750fvi library.

  DESCRIPTION:
  The sketch measures light intensity with one determined BH1750FVI sensor.
  - Connect sensor's pins to microcontroller's I2C bus or I2C default pins
    as described in README.md for used platform accordingly.
  - Leave ADDR pin floating or connected to GND in order to use default address.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "GBJ_BH1750FVI_BASIC 1.0.0"

#include "gbj_bh1750fvi.h"

const unsigned int PERIOD_MEASURE = 3000;  // Time in miliseconds between measurements

gbj_bh1750fvi Light = gbj_bh1750fvi();
// gbj_bh1750fvi Light = gbj_bh1750fvi(gbj_bh1750fvi::CLOCK_100KHZ, true, D2, D1);
// gbj_bh1750fvi Light = gbj_bh1750fvi(gbj_bh1750fvi::CLOCK_400KHZ);


void errorHandler()
{
  if (Light.isSuccess()) return;
  Serial.print("Error: ");
  Serial.print(Light.getLastResult());
  Serial.print(" - ");
  switch (Light.getLastResult())
  {
    case gbj_bh1750fvi::ERROR_ADDRESS:
      Serial.println("Bad address");
      break;

    case gbj_twowire::ERROR_PINS:
      Serial.println("Bad pins");
      break;

    case gbj_bh1750fvi::ERROR_NACK_OTHER:
      Serial.println("Other error");
      break;

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
  Serial.println(gbj_bh1750fvi::VERSION);
  Serial.println("---");

  if (Light.begin())
  {
    errorHandler();
    return;
  }
  Serial.print("Address: 0x");
  Serial.println(Light.getAddress(), HEX);
  Serial.print("Mode: 0x");
  Serial.println(Light.getMode(), HEX);
  Serial.print("Clock: ");
  Serial.print(Light.getBusClock() / 1000);
  Serial.println(" kHz");
  Serial.println("---");
  Serial.println("Light in lux:");
}


void loop()
{
  if (Light.isError()) return;
  Serial.println(Light.measureLight());
  delay(PERIOD_MEASURE);
}

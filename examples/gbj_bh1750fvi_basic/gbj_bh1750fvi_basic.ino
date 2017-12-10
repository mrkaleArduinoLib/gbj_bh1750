/*
  NAME:
  Basic usage of gbj_bh1750fvi library.

  DESCRIPTION:
  The sketch measures light intensity with one determined BH1750FVI sensor.
  - Connect sensor's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.
  - Leave ADDR pin floating or connected to GND in order to use default address.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_bh1750fvi.h"
#define SKETCH "GBJ_BH1750FVI_BASIC 1.0.0"

const unsigned int PERIOD_MEASURE = 3000;      // Time in miliseconds between measurements

gbj_bh1750fvi Light = gbj_bh1750fvi();


void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(GBJ_TWOWIRE_VERSION);
  Serial.println(GBJ_BH1750FVI_VERSION);
  Serial.println("---");

  if (Light.begin())
  {
    Serial.println("Sensor not initiated!");
  }
  else
  {
    Serial.print("Address: 0x");
    Serial.println(Light.getAddress(), HEX);
    Serial.print("Mode: 0x");
    Serial.println(Light.getMode(), HEX);
    Serial.println("---");
    Serial.println("Light in lux:");
  }
}


void loop()
{
  if (Light.isError()) return;
  Serial.println(Light.measureLight());
  delay(PERIOD_MEASURE);
}

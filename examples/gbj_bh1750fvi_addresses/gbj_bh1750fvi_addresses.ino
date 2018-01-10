/*
  NAME:
  Demonstration of the setting sensor's address by ADDR pin.

  DESCRIPTION:
  The sketch sets address of the light sensor according to the current state
  of the ADDR pin.
  - Connect sensor's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.
  - Connect microcontroller's digital pin 2 to sensor's pin ADDR (Addressing)
    in order to set its address programatically.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_bh1750fvi.h"
#define SKETCH "GBJ_BH1750FVI_ADDRESSES 1.0.0"

const unsigned int PERIOD_MEASURE = 3000;     // Time in miliseconds between measurements
const unsigned char PIN_BH1750FVI_ADDR = 2;   // Address pin of the sensor

gbj_bh1750fvi Light = gbj_bh1750fvi();


void setup()
{
  Serial.begin(9600);
  Serial.println(SKETCH);
  Serial.println("Libraries:");
  Serial.println(GBJ_TWOWIRE_VERSION);
  Serial.println(GBJ_BH1750FVI_VERSION);
  Serial.println("---");
  // Set sensor's address pin
  pinMode(PIN_BH1750FVI_ADDR, OUTPUT);
  digitalWrite(PIN_BH1750FVI_ADDR, LOW);

  if (Light.begin(digitalRead(PIN_BH1750FVI_ADDR)))
  {
    Serial.println("Sensor not initiated!");
  }
  else
  {
    Serial.println("Address/Light:");
  }
}


void loop()
{
  if (Light.isError()) return;
  // Change sensor's address
  digitalWrite(PIN_BH1750FVI_ADDR, digitalRead(PIN_BH1750FVI_ADDR) ^ 1);
  Light.setAddress(digitalRead(PIN_BH1750FVI_ADDR));
  Serial.print("0x");
  Serial.print(Light.getAddress(), HEX);
  Serial.print("/");
  if (Light.isSuccess())
  {
    Serial.println(Light.measureLight());
  }
  else
  {
    Serial.print("Error: ");
    Serial.println(Light.getLastResult());
  }
  delay(PERIOD_MEASURE);
}

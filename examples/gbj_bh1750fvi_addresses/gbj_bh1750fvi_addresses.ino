/*
  NAME:
  Demonstration of the setting sensor's address by ADDR pin.

  DESCRIPTION:
  The sketch sets address of the light sensor according to the current state
  of the ADDR pin.
  - Connect sensor's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.
  - Connect microcontroller's digital pin 7 to sensor's pin ADDR (Addressing)
    in order to set its address programatically.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#include "gbj_bh1750fvi.h"
#define SKETCH "GBJ_BH1750FVI_ADDRESSES 1.0.0"

const unsigned int PERIOD_MEASURE = 3000;  // Time in miliseconds between measurements
const unsigned char PIN_BH1750FVI_ADDR = 7;  // Address pin of the sensor

gbj_bh1750fvi Light = gbj_bh1750fvi();


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
  // Set sensor's address pin
  pinMode(PIN_BH1750FVI_ADDR, OUTPUT);
  digitalWrite(PIN_BH1750FVI_ADDR, LOW);

  if (Light.begin(digitalRead(PIN_BH1750FVI_ADDR)))
  {
    errorHandler();
    return;
  }
  else
  {
    Serial.println("Address/Pin/Light:");
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
  Serial.print(digitalRead(PIN_BH1750FVI_ADDR) == HIGH ? "HIGH" : "LOW");
  Serial.print("/");
  if (Light.isError())
  {
    errorHandler();
  }
  else
  {
    Serial.println(Light.measureLight());
  }
  delay(PERIOD_MEASURE);
}

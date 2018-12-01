/*
  NAME:
  Demonstration of the setting sensor's address by ADDR pin.

  DESCRIPTION:
  The sketch sets address of the Sensor sensor according to the current state
  of the ADDR pin.
  - Connect sensor's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.
  - Connect microcontroller's some general digital pin to sensor's pin ADDR (Addressing)
    in order to set its address programatically.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "GBJ_BH1750FVI_ADDRESSES 1.0.0"

#include "gbj_bh1750fvi.h"

const unsigned int PERIOD_MEASURE = 3000;  // Time in miliseconds between measurements

#if defined(ESP8266) || defined(ESP32)
const unsigned char PIN_BH1750FVI_ADDR = D0;  // Address pin of the sensor
#else
const unsigned char PIN_BH1750FVI_ADDR = 7;  // Address pin of the sensor
#endif

gbj_bh1750fvi Sensor = gbj_bh1750fvi();
// gbj_bh1750fvi Sensor = gbj_bh1750fvi(gbj_bh1750fvi::CLOCK_100KHZ, true, D2, D1);
// gbj_bh1750fvi Sensor = gbj_bh1750fvi(gbj_bh1750fvi::CLOCK_400KHZ);


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
    case gbj_bh1750fvi::ERROR_ADDRESS:
      Serial.println("ERROR_ADDRESS");
      break;

    case gbj_bh1750fvi::ERROR_PINS:
      Serial.println("ERROR_PINS");
      break;

    // Arduino, Esspressif specific
#if defined(__AVR__) || defined(ESP8266) || defined(ESP32)
    case gbj_bh1750fvi::ERROR_BUFFER:
      Serial.println("ERROR_BUFFER");
      break;

    case gbj_bh1750fvi::ERROR_NACK_DATA:
      Serial.println("ERROR_PINS");
      break;

    case gbj_bh1750fvi::ERROR_NACK_OTHER:
      Serial.println("ERROR_NACK_OTHER");
      break;

    // Particle specific
#elif defined(PARTICLE)
    case gbj_bh1750fvi::ERROR_BUSY:
      Serial.println("ERROR_BUSY");
      break;

    case gbj_bh1750fvi::ERROR_END:
      Serial.println("ERROR_END");
      break;

    case gbj_bh1750fvi::ERROR_TRANSFER:
      Serial.println("ERROR_TRANSFER");
      break;

    case gbj_bh1750fvi::ERROR_TIMEOUT:
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
  Serial.println(gbj_bh1750fvi::VERSION);
  Serial.println("---");
  // Set sensor's address pin
  pinMode(PIN_BH1750FVI_ADDR, OUTPUT);
  digitalWrite(PIN_BH1750FVI_ADDR, LOW);

  if (Sensor.begin(digitalRead(PIN_BH1750FVI_ADDR)))
  {
    errorHandler("Begin");
    return;
  }
  else
  {
    Serial.println("Address/Pin/Sensor:");
  }
}


void loop()
{
  if (Sensor.isError()) return;
  // Change sensor's address
  digitalWrite(PIN_BH1750FVI_ADDR, digitalRead(PIN_BH1750FVI_ADDR) ^ 1);
  Sensor.setAddress(digitalRead(PIN_BH1750FVI_ADDR));
  Serial.print("0x");
  Serial.print(Sensor.getAddress(), HEX);
  Serial.print("/");
  Serial.print(digitalRead(PIN_BH1750FVI_ADDR) == HIGH ? "HIGH" : "LOW");
  Serial.print("/");
  if (Sensor.isError())
  {
    errorHandler("Address");
  }
  else
  {
    Serial.println(Sensor.measureSensor());
  }
  delay(PERIOD_MEASURE);
}
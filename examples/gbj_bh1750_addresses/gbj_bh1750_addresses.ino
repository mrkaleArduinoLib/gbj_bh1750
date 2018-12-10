/*
  NAME:
  Demonstration of the setting sensor's address by ADDR pin.

  DESCRIPTION:
  The sketch sets address of the Sensor sensor according to the current state
  of the ADDR pin.
  - Connect sensor's pins to microcontroller's I2C bus as described in README.md
    for used platform accordingly.
  - Connect some microcontroller's general digital pin to sensor's pin ADDR (Addressing)
    in order to set its address programatically.

  LICENSE:
  This program is free software; you can redistribute it and/or modify
  it under the terms of the MIT License (MIT).

  CREDENTIALS:
  Author: Libor Gabaj
*/
#define SKETCH "GBJ_BH1750_ADDRESSES 1.0.0"

#include "gbj_bh1750.h"

const unsigned int PERIOD_MEASURE = 3000;  // Time in miliseconds between measurements

#if defined(ESP8266) || defined(ESP32)
const unsigned char PIN_BH1750_ADDR = D0;  // Address pin of the sensor
#else
const unsigned char PIN_BH1750_ADDR = 7;  // Address pin of the sensor
#endif

gbj_bh1750 Sensor = gbj_bh1750();
// gbj_bh1750 Sensor = gbj_bh1750(gbj_bh1750::CLOCK_100KHZ, D2, D1);
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
      Serial.println("ERROR_PINS");
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
  // Set sensor's address pin
  pinMode(PIN_BH1750_ADDR, OUTPUT);
  digitalWrite(PIN_BH1750_ADDR, LOW);

  if (Sensor.begin(digitalRead(PIN_BH1750_ADDR)))
  {
    errorHandler("Begin");
    return;
  }
  else
  {
    Serial.println("Address/Pin/Light");
  }
}


void loop()
{
  if (Sensor.isError()) return;
  // Change sensor's address
  digitalWrite(PIN_BH1750_ADDR, digitalRead(PIN_BH1750_ADDR) ^ 1);
  Sensor.setAddress(digitalRead(PIN_BH1750_ADDR));
  Serial.print("0x" + String(Sensor.getAddress(), HEX) \
    + "/" + (digitalRead(PIN_BH1750_ADDR) == HIGH ? "HIGH" : "LOW") \
    + "/");
  if (Sensor.isError())
  {
    errorHandler("Address");
  }
  else
  {
    if (Sensor.measureLight())
    {
      errorHandler("Measure");
    }
    Serial.println(Sensor.getLightTyp());
  }
  delay(PERIOD_MEASURE);
}

//
// board package Heltec wifi 8 
// works
//
#include <splash.h>

#include "DHTesp.h"
#include <Wire.h>  // Only needed for Arduino 1.6.5 and earlier
#include "heltec.h" // alias for `#include "SSD1306Wire.h"`

// Requires the Heltec Wifi 8 board package and the Heltec library

#ifdef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP8266 ONLY!) // Heltec wifi 8 15 NOV 2020
#error Select ESP8266 board.
#endif

#define LED 15   // labeled d8 on the board

DHTesp dht;

void setup()
{
  pinMode(LED, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  Serial.begin(115200);
//  Serial.println();
//  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)\tHeatIndex (C)\t(F)");
//  String thisBoard= ARDUINO_BOARD;
//  Serial.println(thisBoard);

//  #define DHTpin 14    //D5 of NodeMCU is GPIO14
  dht.setup(14, DHTesp::DHT22); // Connect DHT sensor to GPIO xx

  Heltec.begin(true /*DisplayEnable Enable*/, true /*Serial Enable*/);
  Heltec.display->init();
  //Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_16);

  Heltec.display->drawString(0,0,"DHT22 on pin 14");
  Heltec.display->setFont(ArialMT_Plain_24);
  Heltec.display->display();
}

void blinkLED() {
  digitalWrite(LED, LOW);   // Turn the LED on (Note that LOW is the voltage level
  // but actually the LED is on; this is because
  // it is active low on the ESP-01)
  delay(500);                      // Wait for x seconds
  digitalWrite(LED, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(1000);                      // Wait for x seconds (to demonstrate the active low LED)
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(millis());
  Serial.print("\t");
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperature), 1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperature, humidity, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperature), humidity, true), 1);

  Heltec.display->clear();
  Heltec.display->drawString(0,6,String(temperature, 1)+"C  " + String(temperature, 0) + "%");
  Heltec.display->display();
  
  //delay(2000);
  blinkLED();
}

/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example runs directly on ESP8266 chip.

  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino

  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "DHT.h"
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into D5 on ESP
#define ONE_WIRE_BUS D5

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

#define DHTPIN D6     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);

const int led = 4;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "80d71d7a59024765a5449555ddf098ea"; // DBM
//char auth[] = "4fbcbe517b344942a647667e267568e5";  // Jack

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "lab-i5";
char pass[] = "a1b1c1d1e1";

BlynkTimer timer;

WidgetLED led1(V1);


// V1 LED Widget is blinking
void blinkLedWidget()
{
  if (led1.getValue()) {
    led1.off();
    Serial.println("LED on V1: off");
    //digitalWrite(led, LOW);
  } else {
    led1.on();
    Serial.println("LED on V1: on");
    //digitalWrite(led, HIGH);
  }
}

BLYNK_WRITE(V3) // Button Widget writes to Virtual Pin Vx 
{
  if(param.asInt() == 1) {     // if Button sends 1
    Serial.println("Button V3 pressed");
    digitalWrite(led, LOW);
  } else {
    digitalWrite(led, HIGH);
  }
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  // You can also use:
  // String i = param.asStr();
  // double d = param.asDouble();
  Serial.print("V2 Slider value is: ");
  Serial.println(pinValue);
}

void readtemp()
{
  char Buffer[16];
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = 0.0; //dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    t = -99.0;
    //return;
  }

  sensors.requestTemperatures(); // Send the command to get temperatures
  float t_DS1820 = sensors.getTempCByIndex(0);
  Serial.print("DS18B20 temp = "); 
  Serial.println(t_DS1820);  // getTempCByIndex for celcius

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);

//  Serial.print("Humidity: ");
//  Serial.print(h);
//  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.println("°C");
  //dtostrf(t, 1, 1, Buffer);
  dtostrf(t_DS1820, 1, 1, Buffer); //1 decimal
  Blynk.virtualWrite(V4, Buffer);
  Blynk.virtualWrite(V6, Buffer);  //t_DS1820

//  Serial.print(f);
//  Serial.print(" *F\t");
//  Serial.print("Heat index: ");
//  Serial.print(hic);
//  Serial.print(" *C ");
//  Serial.print(hif);
//  Serial.println(" *F");

}

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
  //Blynk.virtualWrite(V6, (millis() / 1000) % 1);
  Serial.println(millis() / 1000);
  blinkLedWidget();

  readtemp();
  
  
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  dht.begin();
  sensors.begin();

  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every X.Xs
  timer.setInterval(2000L, myTimerEvent);

  pinMode(led, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(led, HIGH);
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}

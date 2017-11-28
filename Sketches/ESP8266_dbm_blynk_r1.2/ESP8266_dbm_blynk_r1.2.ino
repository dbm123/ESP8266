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

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V5, millis() / 1000);
  Blynk.virtualWrite(V6, (millis() / 1000) % 1);
  Serial.println(millis() / 1000);
  blinkLedWidget();
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);

  // Setup a function to be called every second
  timer.setInterval(1000L, myTimerEvent);

  pinMode(led, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
}

void loop()
{
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
}


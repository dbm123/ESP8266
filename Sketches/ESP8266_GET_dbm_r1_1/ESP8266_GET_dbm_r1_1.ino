#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Network settings
const char* ssid     = "lab-i5";
const char* password = "a1b1c1d1e1";
 
void setup () {
 
  Serial.begin(115200);
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
 
    delay(1000);
    Serial.print("Connecting..");
 
  }
  Serial.print("Connected!");
}
 
void loop() {
 
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    Serial.println("About to http GET");
 
    HTTPClient http;  //Declare an object of class HTTPClient

 // http://wifitest.adafruit.com/testwifi/index.html
    http.begin("https://api.coinmarketcap.com/v1/ticker/ethereum/");  //Specify request destination
    //http.begin("http://wifitest.adafruit.com/testwifi/index.html");  //Specify request destination
    int httpCode = http.GET();                                                                  //Send the request
    Serial.println(httpCode); 
 
    if (httpCode > 0) { //Check the returning code
 
      String payload = http.getString();   //Get the request response payload
      Serial.println(payload);                     //Print the response payload
 
    }

    Serial.println("Closing connection");
    http.end();   //Close connection
 
  }
 
  delay(30000);    //Send a request every 30 seconds
 
}

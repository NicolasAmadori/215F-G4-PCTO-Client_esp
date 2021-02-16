#include "WiFi.h"
 
const char* ssid = "ssid";
const char* password =  "password";
//const char* ssid = "iphone";
//const char* password =  "password";
 
void setup() {
 
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);

 Serial.println("Connecting to the WiFi network...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(WiFi.localIP().toString());
  }
 
  Serial.println("Connected to the WiFi network.");
 
}
 
void loop() {}

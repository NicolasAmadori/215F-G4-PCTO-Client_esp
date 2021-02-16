//Library for Wifi Connection
#include "WiFi.h"

//Libraries for OLED Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

bool hotspot = true;
char* ssid;
char* password;

const uint16_t port = 60101;
const char * host = "172.20.10.4";

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);//oled screen object

void setup()
{
  if(!hotspot){
    ssid = "CENSORED";
    password = "CENSORED";
  }
  else{
    ssid = "iphone";
    password = "password";
  }

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(2, OUTPUT);
  
  //OLED screen initialization
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);  
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  
  Serial.begin(115200);//Serial comunication initialization
  Serial.println("1");
 //Wifi comunication initialization
  WiFi.begin(ssid, password);
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.println("Searching WiFi...");
  display.println("Wifi Not Avaible.");
  display.display();
  
  Serial.println("Searching WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi Not Avaible.");
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(500);
  }
  
  digitalWrite(2, HIGH);//LED ON = Connected
  display.clearDisplay();
  
  display.setCursor(0,0);
  
  display.println("Connected.");
  display.println("");
  
  display.print("SSID: ");
  display.println(ssid);
  display.println("");

  
  display.println("Assigned IP: ");
  display.println(WiFi.localIP());
  
  display.display();

  Serial.print("WiFi connected to ");
  Serial.print(ssid);
  Serial.print(" with IP: ");
  Serial.println(WiFi.localIP());

  delay(5000);
}

void loop() 
{
  WiFiClient client;
 
    if (!client.connect(host, port)) {
 
        Serial.println("Connection to host failed");        
        delay(1000);
        return;
    }
 
    Serial.println("Connected to server successful!");
 
    client.print("Hello from ESP32!");
 
    Serial.println("Disconnecting...");
    client.stop();
 
    delay(10000);
}

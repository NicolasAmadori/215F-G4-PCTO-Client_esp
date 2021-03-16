//Library for Wifi Connection
#include "WiFi.h"

//Libraries for OLED Display
//#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Libraries for RFID Reader
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

//Definizione pin per la comunicazione SPI
#define PN532_SCK  (23)
#define PN532_MOSI (17)
#define PN532_SS   (5)
#define PN532_MISO (18)

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//Variables for WiFi Connection
char* ssid = "iphone";
char* password = "password";

const uint16_t port = 60101;
const char * host = "172.20.10.4";
String authorisedID = "4731821042105128";
boolean isAuthenticated = false;
String messaggioProtocollo = "magazziniere|86sr6aMlWN8mEi0LVDA3GVKQ0c2g8qEnB/Y/MBWwBMZk9NF1HeQfgi7gqx2YmdAUy8yUQiiekjRGBd60/TX/f/9aSq2tB1B3+0mwQh0TSk1/szT6N/yOZg46/J2tJddk|add|";

Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);//Interfaccia per l'uso dell'nfc con i pin specificati

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);//oled screen object

void ShowInformation(String s = ""){
  display.clearDisplay();
    display.setCursor(0,0);
    display.println("Connected.");
    display.print("SSID: ");
    display.println(ssid);
    display.print("IP: ");
    display.println(WiFi.localIP());
    if(s != ""){
      display.println("UID Sent:");
      display.print(s);
    }
    display.display();
}

void SendMessage(String message){
  WiFiClient client;
  while(!client.connect(host, port)) {
      Serial.println("Connection to host failed");        
      delay(500);
  }  
  String invio = messaggioProtocollo + message;
  client.print(invio);
  client.stop();
  Serial.print("Message sent: ");
  Serial.println(invio);
  
  ShowInformation(message);
}

void WifiConnection(){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("Searching WiFi...");
  display.println("Wifi Not Avaible.");
  display.display();
  
  //Wifi comunication initialization
  WiFi.begin(ssid, password);

  Serial.println("Searching WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wifi Not Avaible.");
    digitalWrite(2, HIGH);
    delay(200);
    digitalWrite(2, LOW);
    delay(500);
  }
  
  display.println("Connected to Wifi.");
  digitalWrite(2, HIGH);//LED ON = Connected
  ShowInformation();
}

void ScreenInitialization(){
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
  display.setTextColor(WHITE);
  display.display();
}

void NFCReaderInitilization(){
    while (!Serial) delay(10);
  Serial.println("Reading stage:");
  nfc.begin(); //Instaurazione 

  uint32_t versiondata = nfc.getFirmwareVersion(); //Versione
  
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  //Numero massimo di tentativi per leggere un tag
  nfc.setPassiveActivationRetries(0xFF);
  
  //Configurazione per la lettura di tag RFID
  nfc.SAMConfig();
  
  Serial.println("Waiting for an ISO14443A tag");
}

String ReadTag(boolean isAuthenticated = true){
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  //Vettore per la memorizzazione dello UID
  uint8_t uidLength;        //Lunghezza dello UID, può variare da 4 Byte (Mifare Classic) a 7 Byte (Mifare Ultralight)
  
  Serial.println("Reading...");
  //success == true se la lettura è andata a buon fine
  //success == false se non ha trovato tag o se la lettura non è andata a buon fine
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  //Caso di lettura corretta
  if (success) {
    String uidTotale = "";
    for (uint8_t i=0; i < uidLength; i++){
        uidTotale = uidTotale + uid[i];
      }

    if(isAuthenticated){
      SendMessage(uidTotale);
    }          
    //Aspetta un secondo prima di continuare
    delay(500);
    return uidTotale;
  }
  else
  {
    Serial.println("Timed out waiting for a tag");//Caso in cui la lettura del pn532 vada in time out
  }
}



void setup(void) {
  Serial.begin(9600); //Collegamento seriale (9600 baud)
  
  pinMode(2, OUTPUT);// initialize digital pin LED_BUILTIN as an output.
  
  ScreenInitialization();//SSD1306 Screen Initialization
      
  NFCReaderInitilization();//NFC Reader Initialization

  display.clearDisplay();
  display.println("User not authenticated.");
  display.println("Scan your CIE.");
  display.display();
  
  String id;
  do {
    id = ReadTag(false);
    if(authorisedID != id){
      Serial.println("CIE denied, try again.");
      
      display.clearDisplay();
      display.setCursor(0,0);
      display.println("CIE denied, try again.");
      display.display();
    }
  } while (authorisedID != id);
  display.clearDisplay();
  display.setCursor(0,0);
  display.println("CIE approved!");
  display.display();
  Serial.println("CIE approved!");
  isAuthenticated = true;
  sleep(2);//Aspetta 2 secondi
  
  WifiConnection();//WiFi Setup and Connection
  ShowInformation();
}


void loop(void) {
  ReadTag();
}

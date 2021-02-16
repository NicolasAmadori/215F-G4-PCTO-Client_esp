#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>

//Definizione pin per la comunicazione SPI
#define PN532_SCK  (2)
#define PN532_MOSI (4)
#define PN532_SS   (5)
#define PN532_MISO (18)

//Interfaccia per l'uso dell'nfc con i pin specificati
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

void setup(void) {
  Serial.begin(9600); //Collegamento seriale (9600 baud)
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

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  //Vettore per la memorizzazione dello UID
  uint8_t uidLength;        //Lunghezza dello UID, può variare da 4 Byte (Mifare Classic) a 7 Byte (Mifare Ultralight)
  
  Serial.println("reading...");
  //success == true se la lettura è andata a buon fine
  //success == false se non ha trovato tag o se la lettura non è andata a buon fine
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  //Caso di lettura corretta
  if (success) {
    Serial.println("Current tag values");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(uid[i]); //Mostro nel monitor seriale lo UID
    }
    Serial.println("");
  //Aspetta un secondo prima di continuare
  delay(1000);
  }
  else
  {
    //Caso in cui la lettura del pn532 vada in time out
    Serial.println("Timed out waiting for a tag");
  }
}

# 215F-G4-PCTO-Client_esp
Eseguibile esp per client di gestione magazzino con lettori rfid

Contenuto dei vari file:

primo_test_schermo.ino  
Questo progetto arduino contiene la prima prova di utilizzo dello schermo SSD1306 integrato sull'esp32 TTGO

connessione_wifi.ino  
Questo progetto arduino contiene il codice con la prova di connessione ad una rete wifi (verificare unicamente la riuscita della connessione)

wifiClient.ino  
Questo progetto arduino contiene la parte della connessione wifi e l'invio di messaggi tramite protocollo tcp al server con la porta prestabilita 60101 (porta di test). In questo progetto l'esp manda un messaggio al server e chiude la connessione con lo stesso ogni 10 secondi.

Client_Wifi_Con_schermo.ino  
Questo progetto arduino contiene la somma dei 3 progetti precedenti, combinando la connessione wifi a l'utilizzo dello schermo per visualizzare le informazioni importanti. Inoltre viene utilizzato il led integrato nell'esp per mostrare lo stato della connessione wifi (connesso: luce fissa, non connesso: luce lampeggiante)

TCP_Server.py  
Questo progetto python contiene un esempio primordiale di server tcp che mostra semplicemente i messaggi ricevuti dal client esp, creato e utilizzato solo a scopi di test.

esp_script_read.ino  
Questo progetto arduino contiene il codice per la lettura degli UID dei tag utilizzando il modulo pn532 con il protocollo SPI.  Inoltre è presente un processo di autenticazione attraverso la propria carta di identità elettronica, per assicurarsi che il lettore venga utilizzato solo dalla persona autorizzata.

Wifi_Schermo_RFID.ino  
Questo progetto è la fusione di esp_script_read.ino  e Client_Wifi_Con_schermo.ino.
Per cui ha la funzionalità di connessione alla rete WiFi, la possibilità di utilizzare lo schermo integrate nell'esp e la connettività al lettore rfid di schede.
Questo progetto parte cercando di collegarsi al wifi, una volta fatto ciò bisogna autenticarsi con una carta di identità elettronica. Una volta autenticati si potrà iniziare a scansionare tag rfid che verranno spediti al server tcp presente nella stessa rete a cui è collegato l'esp.

TCP_Client.py  
Questo file contiene un esempio di client TCP in python per provare la ricezione dell'esp di pacchetti dalla rete nella porta 80.

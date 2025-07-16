Copyright (c) 2025 Enno Winch
 
// This file is part of the [Oracle SmartCity - Proxima] project and is licensed under the MIT License.
//See the "LICENSE" file in the root directory for details.

// the following "/[...]" or "(/[...]) will be used to explain more in depth or to lead to other explicit resources.

#include <WiFi101.h>
WiFiServer server(80);

float wige = 8;
bool anaus = true;
bool einmalausfuehren = true;
int relaisPin = 5; // !Digitaler! GPIO-PIN für Relay (in diesem Fall "5" /siehe Dokumentation: "Wiring-RX")
float humidity = 0.0;
float temperature = 0.0;
char ssid[] = "INDU4GY"; // Dein Netzwerk Name/SSID
char pass[] = "indu8764"; // Dein Password 

IPAddress ip(192, 168, 1, 200);     // Server-IP (Sensor)
IPAddress gateway(192, 168, 1, 1);  // Router-IP
IPAddress subnet(255, 255, 255, 0);

void setup() {
  // put your setup code here, to run once:
  pinMode(relaisPin, OUTPUT); //Relais Output definition
  Serial.begin(9600);
  while (!Serial);

  WiFi.config(ip, gateway, subnet); // Setzen fester IP 
  WiFi.begin(ssid, pass);

  Serial.print("Verbinde mit WLAN...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nVerbunden!"); // Verbindung zwischen Arduino & Netzwerk erfolgreich!
  Serial.print("IP-Adresse (Server): ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client) {
    if (einmalausfuehren == true){
      Serial.println("Client verbunden!"); // Verbinden mit Sensor- (/RX-) Arduino erfolgreich!
      einmalausfuehren = false;
    }
    

  String empfangen = client.readStringUntil('\n');  // Empfängt die ganze Zeile
  empfangen.trim();  // Entfernt Leerzeichen und Zeilenumbrüche

  Serial.print("Empfangen: '");
  Serial.print(empfangen);
  Serial.println("'");

  // Schlüsselwörter finden
  int lfPos = empfangen.indexOf("Luftfeuchtigkeit:");
  int tPos  = empfangen.indexOf("Temperatur:");

  if (lfPos != -1 && tPos != -1) {
    // Luftfeuchtigkeit extrahieren
    int lfStart = lfPos + String("Luftfeuchtigkeit:").length();
    int lfEnd   = empfangen.indexOf(',', lfStart);
    String lfStr = empfangen.substring(lfStart, lfEnd);
    lfStr.trim();

    // Temperatur extrahieren
    int tStart = tPos + String("Temperatur:").length();
    String tStr = empfangen.substring(tStart);
    tStr.trim();

    // In float umwandeln – mit englischen Variablennamen
    humidity = lfStr.toFloat();
    temperature = tStr.toFloat();

    Serial.print("Luftfeuchtigkeit: "); // Anzeigen der <imp.> (/importierten) Luftfeuchtigkeit-Werten von TX-Arduino
    Serial.println(humidity);

    Serial.print("Temperatur: "); // Anzeigen der <imp.> (/importierten) Temperatur-Werten von TX-Arduino
    Serial.println(temperature);
  } else {
    Serial.println("Fehler: 'Luftfeuchtigkeit' oder 'Temperatur' nicht gefunden!");
  }


    //float humidity = empfangen.toFloat();
    //Serial.print("Empfangen: ");
    //Serial.println(empfangen);

    client.println("Nachricht vom Server empfangen!");
    client.stop();
    

    if(humidity >= 80){ // - Schwellenwert überschritten/gleich [>/=] -
      anaus = false; // - An/Aus Relais
    }
    else{
      anaus = true;
    }

    if(anaus == true) {
      digitalWrite(relaisPin, HIGH); // - Relais: ON / OFF - [bei TRUE bzw. FAlSE] (siehe bei: "else if")
    }
    else if(anaus == false) {
      digitalWrite(relaisPin, LOW);
    }
    else {
        Serial.println("Error");
    }

  }
    
} 
 //The code was developed by Luise J. & Enno W. during their student internship at Oracle. All rights reserged --> LICENSE 

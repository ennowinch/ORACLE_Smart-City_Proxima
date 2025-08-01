Copyright (c) 2025 Enno Winch
 
// This file is part of the [Oracle SmartCity - Proxima] project and is licensed under the MIT License.
// See the "LICENSE" file in the root directory for details.

// the following "/[...]" or "(/[...])" will be used to explain more in depth or to lead to other explicit resources.

#include <WiFi101.h>
WiFiServer server(80);

float wige = 8;
bool anaus = true;
bool einmalausfuehren = true;
int relaisPin = 5; // !Digitaler! GPIO-PIN für Relay (in diesem Fall "5" /siehe Dokumentation: "Wiring-RX")
float humidity = 0.0;
float temperature = 0.0;
char ssid[] = "INDU4GY"; // Your Network Name/SSID
char pass[] = "indu8764"; // Your Password

IPAddress ip(192, 168, 1, 200);     // Server-IP (Sensor)
IPAddress gateway(192, 168, 1, 1);  // Router-IP
IPAddress subnet(255, 255, 255, 0);

void setup() {
  // put your setup code here, to run once:
  pinMode(relaisPin, OUTPUT); //Relais Output definition
  Serial.begin(9600);
  while (!Serial);

  WiFi.config(ip, gateway, subnet); // Set static IP
  WiFi.begin(ssid, pass);

  Serial.print("Verbinde mit WLAN...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nVerbunden!"); // Connection between Arduino & network successful!
  Serial.print("IP-Adresse (Server): ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (client) {
    if (einmalausfuehren == true){
      Serial.println("Client verbunden!"); // Connection to sensor (/RX) Arduino successful!
      einmalausfuehren = false;
    }
    

  String empfangen = client.readStringUntil('\n'); // Receives the entire line
  empfangen.trim();  // Removes spaces and line breaks
  Serial.print("Empfangen: '");
  Serial.print(empfangen);
  Serial.println("'");

  // Find keywords
  int lfPos = empfangen.indexOf("Luftfeuchtigkeit:");
  int tPos  = empfangen.indexOf("Temperatur:");

  if (lfPos != -1 && tPos != -1) {
    // Extract humidity
    int lfStart = lfPos + String("Luftfeuchtigkeit:").length();
    int lfEnd   = empfangen.indexOf(',', lfStart);
    String lfStr = empfangen.substring(lfStart, lfEnd);
    lfStr.trim();

    // Extract temperature
    int tStart = tPos + String("Temperatur:").length();
    String tStr = empfangen.substring(tStart);
    tStr.trim();

    // Convert to float – with English variable names
    humidity = lfStr.toFloat();
    temperature = tStr.toFloat();

    Serial.print("Luftfeuchtigkeit: "); // Display the <imp.> (/imported) humidity values from TX-Arduino
    Serial.println(humidity);

    Serial.print("Temperatur: "); // Display the <imp.> (/imported) temperature values from TX-Arduino
    Serial.println(temperature);
  } else {
    Serial.println("Fehler: 'Luftfeuchtigkeit' oder 'Temperatur' nicht gefunden!");
  }


    //float humidity = empfangen.toFloat();
    //Serial.print("Empfangen: ");
    //Serial.println(empfangen);

    client.println("Nachricht vom Server empfangen!");
    client.stop();
    

    if(humidity >= 80){ // - (Relative humidity in percent (RHP) /) Threshold value exceeded/equal to [>/=] -
      anaus = false; // - On/Off relay
    }
    else{
      anaus = true;
    }

    if(anaus == true) {
      digitalWrite(relaisPin, HIGH); // - Relay: ON / OFF - [if TRUE or FALSE] (see: “else if”)
    }
    else if(anaus == false) {
      digitalWrite(relaisPin, LOW);
    }
    else {
        Serial.println("Error");
    }

  }
    
} 
 // The code was developed by Luise J. & Enno W. during their student internship at Oracle. All rights reserved --> see "LICENSE" for details.

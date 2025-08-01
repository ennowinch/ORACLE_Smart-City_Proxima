#include "DHT.h" // Verwendung der DHT-11 Sensorbibliothek
#include <WiFi101.h> // WiFi Bibliothek

#define DHTPIN 2      // Pin, an dem der DHT-11 angeschlossen ist (/Sensor)
#define DHTTYPE DHT11 // DHT 11 Sensor
const char* ssid = "INDU4GY"; // (YOUR SSID)
const char* password = "indu8764"; // (YOUR PASSWORD)
char server[] = "192.168.1.200";
bool einmalausfuehren = true;

WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  
  dht.begin();
  Serial.println("DHT11 Sensor starten...");

  WiFi.begin(ssid, password);
  Serial.print("Verbinde mit WLAN...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nVerbunden!");
}

void loop() {
  delay(2000); // 2 Sekunden zwischen den Messungen

  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Fehler beim Lesen vom Sensor!");
    return;
  }

  Serial.print("Luftfeuchtigkeit: ");
  Serial.print(humidity);
  Serial.print(" %\t");
  Serial.print("Temperatur: ");
  Serial.print(temperature);
  Serial.println(" °C");

  if(humidity >= 80){
    Serial.println("Attention: very high humidity!");
  }

  if (client.connect(server, 80)) {
    if (einmalausfuehren == true){
      Serial.println("Mit Server verbunden");
      einmalausfuehren = false;
    }
    client.print("Luftfeuchtigkeit: ");
    client.print(humidity);
    client.print(" %\t");
    client.print("Temperatur: ");
    client.print(temperature);
    client.println(" °C");
    String antwort = client.readStringUntil('\n');
    client.stop();

  } else {
    Serial.println("Verbindung fehlgeschlagen");
  }

  delay(2000); // alle 2 Sekunden neu senden
}
// The code was developed by Luise J. during her student internship at Oracle. All rights reserved --> see "LICENSE" for details.

# ORACLE SmartCity : "Proxima" (2025)

A renewable City model of a city of the future with intelligent integration into the Cloud, which was developed as part of the student internship in 2025 by Luise J., Smilla F. and Enno W. at Oracle® (DE). In 4 Days we developed the following code for an Arduino (MKR-1000 WiFi) and a Raspberry Pi 3 (Model_B Plus Rev 1.3). 

Structure: 
1) WeatherStation - Arduino (_so called: "temperature.ino"_)  / Sensor Array (Humidity + Temperature)
2) PowerPlant Regulator - Arduino (_so called: "Windanlage_Master.ino"_)  / Relay to switch on/off the Wind Turbine  
3) SmartTrain - Rasberry Pi 3 (_so called: "Bluetooth_Connection_LEGO_Hub"_)  / Drivers for Lego (Train) Hub


1. Weather Station - TX-Arduino (Arduino (Type): MKR-1000 WiFi)
   Expl. : Transmission-Arduino [--> TX] wired up to a combination sensor [Humidity (+) Temperature] (DHT 11), that reads the data and displays in the readable form in the Serial Monitor and is able to export them over WiFI to the "RX-Arduino" [/_Recieving Arduino_ (PowerPlant Regulator Arduino)].
   Requirments : Arduino with WiFi capabilities, Arduino IDE [/_"Integratet Develloper Environment"_] and an Arduino IDE Library (AIL) AddOn, called: "Wifi101.h" to share the read out data over WLAN to other instances.
   Build. : 

3. Power Plant Regulation - RX-Arduino (Arduino (Type): MKR-1000 WiFi)
   Expl. : Receiving-Arduino [--> RX] wired up to a Relay [max. 5V Operating Voltage], that switches the LEGO M-Motor on/off to simlulate different weather conditions. The relay has a max. switchable Voltage of 230V at 50-60 Hz and needs 5V at the (+)-Port, Ground at the (-)-Port to operate and about 3,3V on the (S)-Port [S = Data Port] to switch the relay on / off --> _more under: "Power-Plant-(RX) / _ Branch_. It imports the data via WiFi and reacts depending on the set switch (_so called: "

 -- Imprint --

## License 

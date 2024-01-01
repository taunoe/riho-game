/*
Player is Client

File:    main.cpp
Started: 03.12.2023
GitHub:  https://github.com/taunoe/game-show
Copyright 2023 Tauno Erik


https://arduino-pico.readthedocs.io/en/latest/
*/
#include <Arduino.h>
#include <WiFi.h>

// Wifi
const char* ssid = "Tauno";
const char* password = "Erik2023";

// Host
const char* HOSTNAME = "Game";
const uint16_t PORT = 4242;  // uint16_t

WiFiMulti multi;


void setup() {
  Serial.begin(115200);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  multi.addAP(ssid, password);

  if (multi.run() != WL_CONNECTED) {
    Serial.println("Unable to connect to network, rebooting in 10 seconds...");
    delay(10000);
    rp2040.reboot();
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  static bool wait = false;

  Serial.print("connecting to ");
  Serial.print(HOSTNAME);
  Serial.print(':');
  Serial.println(PORT);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(HOSTNAME, PORT)) {
    Serial.println("connection failed");
    delay(5000);
    return;
  }

  // This will send a string to the server
  Serial.println("sending data to server");
  if (client.connected()) {
    client.println("hello from RP2040");
  }

  // wait for data to be available
  uint64_t timeout = millis();
   
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      delay(60000);
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  Serial.println("receiving from remote server");
  // not testing 'client.connected()' since we do not need to send data here
  while (client.available()) {
    char ch = static_cast<char>(client.read());
    Serial.print(ch);
  }

  // Close the connection
  Serial.println();
  Serial.println("closing connection");
  client.stop();

  if (wait) {
    delay(300000);  // execute once every 5 minutes, don't flood remote service
  }
  wait = true;
}

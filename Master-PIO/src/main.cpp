/*
Master is Server

File:    main.cpp
Started: 03.12.2023
GitHub:  https://github.com/taunoe/game-show
Copyright 2023 Tauno Erik

char MY_SSID[] = "Tauno";
char MY_PASS[] = "Erik2023";

https://arduino-pico.readthedocs.io/en/latest/
https://github.com/tigoe/Wifi_examples
*/
#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>

// Wifi
const char* ssid = "Tauno";
const char* password = "Erik2023";
// Host
const char* HOSTNAME = "Game";
const uint PORT = 4242;

WiFiServer server(PORT);

// whether or not the client was connected previously
bool alreadyConnected = false;

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.setHostname(HOSTNAME);

  Serial.printf("Connecting to '%s' with '%s'\n", ssid, password);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.printf("\nConnected to WiFi\n\nConnect to server at %s:%d\n", WiFi.localIP().toString().c_str(), PORT);

  server.begin();
}

void loop() {
  static int i;
  delay(1000);
  Serial.printf("--loop %d\n", ++i);
  delay(10);

  // wait for a new client:
  WiFiClient client = server.available();

  if (!client) {
    return;
  }

  // when the client sends the first byte, say hello:
  if (client) {
    if (!alreadyConnected) {
      // clear out the input buffer:
      client.flush();
      Serial.println("We have a new client");
      client.println("Hello, client!");
      alreadyConnected = true;
    }

    if (client.available() > 0) {
      // read the bytes incoming from the client:
      char thisChar = client.read();
      // echo the bytes back to the client:
      client.write(thisChar);
      // echo the bytes to the server as well:
      Serial.write(thisChar);
    }
  }

/*
  client.println("Type anything and hit return");
  while (!client.available()) {
    delay(10);
  }

  String req = client.readStringUntil('\n');
  Serial.println(req);
  client.printf("Hello from Pico-W\r\n");
  client.flush();
  */
}

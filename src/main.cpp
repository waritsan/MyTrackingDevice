#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WifiLocation.h>

const char* googleApiKey = "";
WifiLocation location(googleApiKey);
const char* host = "dweet.io";
const int httpPort = 80;
String thingName = WiFi.macAddress();
long lastMsg = 0;

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager;
  wifiManager.autoConnect();
}

void loop() {
  long now = millis();
  if (now - lastMsg > 10000) {
    lastMsg = now;
    WiFiClient client;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      client.stop();
      return;
    }
    location_t loc = location.getGeoFromWiFi();
    Serial.printf("(%f, %f)\n", loc.lat, loc.lon);
    client.print(String("GET /dweet/for/") + thingName + "?your_latitude=" + loc.lat + "&your_longitude=" + loc.lon + " HTTP/1.1\r\n" +
      "Host: " + host + "\r\n" +
      "Connection: close\r\n\r\n"
    );
    while (client.connected() || client.available()) {
      if (client.available()) {
        String line = client.readStringUntil('\n');
        Serial.println(line);
      }
    }
     client.stop();
  }
}


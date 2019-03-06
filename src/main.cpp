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
  if (now - lastMsg > 5000) {
    lastMsg = now;
    WiFiClient client;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      client.stop();
      return;
    }
    location_t loc = location.getGeoFromWiFi();
    char latBuff[20];
    sprintf(latBuff, "%f", loc.lat);
    Serial.println(latBuff);
    char lonBuff[20];
    sprintf(lonBuff, "%f", loc.lon);
    Serial.printf("(%s, %s)\n", latBuff, lonBuff);
    client.print(String("GET /dweet/for/") + thingName + "?your_latitude=" + latBuff + "&your_longitude=" + lonBuff + "&timestamp=" + "2019-03-06T19:38:47.300Z" + now/1000 + " HTTP/1.1\r\n" +
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


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

String thingName = WiFi.macAddress();
const char* host = "dweet.io";
const int httpPort = 80;

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager;
  wifiManager.autoConnect();
}

void loop() {
  WiFiClient client;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    client.stop();
    return;
  }
  int latitude = random(-85, 85);
  int longitude = random(-180, 180);
  Serial.printf("(%d, %d)\n", latitude, longitude);
  client.print(String("GET /dweet/for/") + thingName + "?your_latitude=" + latitude + "&your_longitude=" + longitude + " HTTP/1.1\r\n" +
    "Host: " + host + "\r\n" +
    "Connection: close\r\n\r\n"
  );
  delay(30000);
}
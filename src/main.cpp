#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <WifiLocation.h>
#include <dweetESP8266.h>

const char* googleApiKey = "YOUR API KEY HERE";
char latKey[] = "your_latitude";
char lonKey[] = "your_longitude";
char thingName[20];
WifiLocation location(googleApiKey);
dweet dweetClient;

void setup() {
  Serial.begin(9600);
  WiFiManager wifiManager;
  wifiManager.autoConnect();
  WiFi.macAddress().toCharArray(thingName, 20);
}

void loop() {
  location_t loc = location.getGeoFromWiFi();
  Serial.printf("\n(%f, %f)\n", loc.lat, loc.lon);
  dweetClient.add(latKey, String(loc.lat));
  dweetClient.add(lonKey, String(loc.lon));
  dweetClient.sendAll(thingName);
  delay(30000);
}


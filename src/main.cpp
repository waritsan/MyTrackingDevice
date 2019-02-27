#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* ssid = "santaputra";
const char* password = "1234567890";
String thingName = WiFi.macAddress(); // 5C:CF:7F:1A:FE:46
const char* host = "dweet.io";
const int httpPort = 80;

void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("(latitude, longitude)");
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
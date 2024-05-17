#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

// Nhìn từ đít lên 
// Encoder bánh phải: A 27, B 26
// Encoder bánh trái: A: 25, B 33

// Bánh phải: OUTA: 16 , OUTB: 17 
////Quay ngược 16:HIGH, 17: LOW
// Bánh trái: OUTC: 18 , OUTD: 19
////Quay 


// Sửa SSID và password của mình
const char* ssid = "WiFi 2.4";
const char* password = "lichsuthicho";

AsyncWebServer server(80);

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! I am ESP32.");
  });

  AsyncElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  Serial.println("HTTP server started");


// bên trên không được sửa
// Code từ dòng này trở xuống

  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

}

void loop(void) {
  digitalWrite(18, 1);
  digitalWrite(19, 0);
  digitalWrite(16, 1);
  digitalWrite(17, 0);
  delay(3000);
  digitalWrite(18,0);
  digitalWrite(19,1);
  digitalWrite(16,0);
  digitalWrite(17,1);
  delay(3000);
}
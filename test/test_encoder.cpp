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


#define ENCA 26 // YELLOW
#define ENCB 27 // WHITE
// #define PWM 5
#define IN2 16
#define IN1 17

const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;

volatile int posi = 0; // specify posi as volatile: https://www.arduino.cc/reference/en/language/variables/variable-scope-qualifiers/volatile/
long prevT = 0;
float eprev = 0;
float eintegral = 0;

// Sửa SSID và password của mình
const char* ssid = "WiFi 2.4";
const char* password = "lichsuthicho";

AsyncWebServer server(80);

void motor_backward(int pwr, int in1, int in2)
{
  ledcWrite(pwmChannel, pwr);
  digitalWrite(in1, 0);
}
void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    posi++;
  }
  else{
    posi--;
  }
}

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

  pinMode(ENCA,INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);

  ledcSetup(pwmChannel, freq, resolution);
  ledcSetup(1, freq, resolution);
  
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(IN1, pwmChannel);
  ledcAttachPin(IN2, 1);

}

void loop(void) {

  // Read the position
  int pos = 0; 
  noInterrupts(); // disable interrupts temporarily while reading
  pos = posi;
  interrupts(); // turn interrupts back on
  
  if (pos < 1200)
    motor_backward(200, IN1, IN2);
  else
    motor_backward(0, IN1, IN2);

  
  

  // Serial.print(target);
  // Serial.print(" ");
  Serial.print(pos);
  Serial.println();
}


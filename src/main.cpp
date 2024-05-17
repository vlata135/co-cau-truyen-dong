#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>


#define MOTOR_RIGHT_IN1 16
#define MOTOR_RIGHT_IN2 17
#define MOTOR_RIGHT_EN1 26
#define MOTOR_RIGHT_EN2 27

#define MOTOR_LEFT_IN1 18
#define MOTOR_LEFT_IN2 19
#define MOTOR_LEFT_EN1 25
#define MOTOR_LEFT_EN2 33

#define channel_0 0
#define channel_1 1
#define channel_2 2
#define channel_3 3

// Se phat trien thanh class

volatile uint8_t encoder_right = 0;
volatile uint8_t encoder_left = 0;

// Sửa SSID và password của mình
const char* ssid = "WiFi 2.4";
const char* password = "lichsuthicho";
AsyncWebServer server(80);
void readEncoder_right();
void readEncoder_left();
void move_forward(int pwr, int in1, int in2);


void setup()
{
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
    server.on("/velocity", );

    AsyncElegantOTA.begin(&server);    // Start ElegantOTA
    server.begin();
    Serial.println("HTTP server started");


// bên trên không được sửa
// Code từ dòng này trở xuống

// setup motor
    pinMode(MOTOR_RIGHT_IN1, OUTPUT);
    pinMode(MOTOR_RIGHT_IN2, OUTPUT);
    pinMode(MOTOR_RIGHT_EN1, INPUT);
    pinMode(MOTOR_RIGHT_EN2, INPUT);

    pinMode(MOTOR_LEFT_IN1, OUTPUT);
    pinMode(MOTOR_LEFT_IN2, OUTPUT);
    pinMode(MOTOR_LEFT_EN1, INPUT);
    pinMode(MOTOR_LEFT_EN2, INPUT);

    
// setup PWM channel
    ledcSetup(channel_0, 30000, 8);
    ledcAttachPin(MOTOR_RIGHT_IN1, channel_0);
    ledcSetup(channel_1, 30000, 8);
    ledcAttachPin(MOTOR_RIGHT_IN2, channel_1);
    ledcSetup(channel_2, 30000, 8);
    ledcAttachPin(MOTOR_LEFT_IN1, channel_2);
    ledcSetup(channel_3, 30000, 8);
    ledcAttachPin(MOTOR_LEFT_IN2, channel_3);

// setup interupt for encoder
    attachInterrupt(digitalPinToInterrupt(MOTOR_RIGHT_EN1),readEncoder_right,RISING);
    attachInterrupt(digitalPinToInterrupt(MOTOR_LEFT_EN1),readEncoder_left,RISING);
}

void loop()
{
    uint8_t pos_right = 0;
    uint8_t pos_left = 0;
    noInterrupts(); // disable interrupts temporarily while reading
    pos_right = encoder_right;
    pos_left = encoder_left;
    interrupts(); // turn interrupts back on

    if( pos_right < 1200)
    {
        move_forward(200, 16, 17);
    }
    else
    {
        move_forward(0, 16, 17);
    }
}

void readEncoder_right()
{
    int right = digitalRead(26);
    if(right > 0)
    {
        encoder_right++;
    }
    else
    {
        encoder_right--;
    }
}
void readEncoder_left()
{
    int left = digitalRead(25);
    if(left > 0)
    {
        encoder_left++;
    }
    else
    {
        encoder_left--;
    }
}
void move_forward(int pwr, int in1, int in2)
{
    ledcWrite(0, pwr);
    digitalWrite(, 0);
}
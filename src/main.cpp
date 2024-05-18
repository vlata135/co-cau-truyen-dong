#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>
// #include<WebServer.h>

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

volatile int encoder_right = 0;
volatile int encoder_left = 0;

long prevT = 0;
float eprev = 0;
float eintegral = 0;


// Sửa SSID và password của mìnhd
const char* ssid = "WiFi 2.4";
const char* password = "lichsuthicho";
AsyncWebServer server(80);

void readEncoder_right();
void readEncoder_left();
void move_forward(int pwr, int channel, int in2);
void move_backward(int pwr, int channel, int in2);
void print_pos(AsyncWebServerRequest *request)
{
    request->send(200, "text/plain", String(encoder_right));
}


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
    server.on("/velocity", print_pos);

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
    attachInterrupt(digitalPinToInterrupt(MOTOR_LEFT_EN2),readEncoder_left,RISING);
}

void loop()
{
    int target = 1200;
    // PID constants
    float kp = 8;
    float kd = 0.0025;
    float ki = 5;

    // time difference
    long currT = micros();
    float deltaT = ((float) (currT - prevT))/( 1.0e6 );
    prevT = currT;

    // Read the position
    int pos_right = 0;
    int pos_left = 0;
    noInterrupts(); // disable interrupts temporarily while reading
    pos_right = encoder_right;
    pos_left = encoder_left;
    interrupts(); // turn interrupts back on
    

    // error
  int e = pos_right - target;

  // derivative
  float dedt = (e-eprev)/(deltaT);

  // integral
  eintegral = eintegral + e*deltaT;

  // control signal
  float u = kp*e + kd*dedt + ki*eintegral;
  // motor power
  float pwr = fabs(u);
  if( pwr > 255 ){
    pwr = 255;
  }

    // if( u < 0)
    // {
    //     move_forward(pwr, channel_0, 17);
    //     move_forward(pwr, channel_2, MOTOR_LEFT_IN2);
    // }
    // if (u>0)
    // {
    //     move_backward(pwr, channel_1, 16);
    //     move_backward(pwr, channel_3, MOTOR_LEFT_IN1);
    // }

    // if( pos_right < 1200)
    // {
    //     move_forward(200, channel_0, 17);
    // }
    // else
    // {
    //     move_forward(0, channel_0, 17);
    // }
    Serial.print(pwr);
    Serial.print(" ");
    Serial.print(pos_right);
    Serial.print(" ");
    Serial.print(pos_left);
    Serial.println();

}

void readEncoder_right()
{
    int right = digitalRead(MOTOR_RIGHT_EN2);
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
    int left = digitalRead(MOTOR_LEFT_EN1);
    if(left > 0)
    {
        encoder_left++;
    }
    else
    {
        encoder_left--;
    }
}
void move_forward(int pwr, int channel, int in2)
{
    ledcWrite(channel, pwr);
    digitalWrite(in2, 0);
}
void move_backward(int pwr, int channel, int in2)
{
    ledcWrite(channel, pwr);
    digitalWrite(in2, 0);
}
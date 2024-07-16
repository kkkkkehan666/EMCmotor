#include <WiFi.h>
#include <WebServer.h>
#include "Freenove_4WD_Car_For_ESP32.h"
// Replace with your network credentials
const char* ssid = "PA Internal";
const char* password = "physics2014";

int speed;

// Create a web server object that listens for HTTP request on port 80
WebServer server(80);

// Motor control pins
const int motor1Pin1 = 5; // IN1
const int motor1Pin2 = 18; // IN2
const int motor2Pin1 = 19; // IN3
const int motor2Pin2 = 21; // IN4

void setup() {
  Serial.begin(115200);
  PCA9685_Setup(); 

  // Initialize motor control pins as outputs
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi. IP address: ");
  Serial.println(WiFi.localIP());

  // Define the routes
  server.on("/", handleRoot);
  server.on("/f", handleForward);
  server.on("/b", handleBackward);
  server.on("/l", handleLeft);
  server.on("/r", handleRight);
  server.on("/s", handleStop);

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

int getSpeedFromRequest() {
  if (server.hasArg("speed")) {
    // Serial.println("hello");
    return constrain(server.arg("speed").toInt(), 0, 4095);
    // Ensure speed is within valid range
  }else{
    return 1000;
  } // Default speed if not specified
}

void loop() {
  // Handle client requests
  server.handleClient();

  speed = getSpeedFromRequest();
}

void handleRoot() {
  server.send(200, "text/plain", "Welcome to the Freenove Car Control!");
}


void handleForward() {
  Motor_Move(-speed, -speed, -speed, -speed);     //go forward
  delay(1000);
  server.send(200, "text/plain", "Moving Forward");
}

void handleBackward() {
  Motor_Move(speed, speed, speed, speed); //go back
  delay(1000);
  server.send(200, "text/plain", "Moving Backward");
}

void handleLeft() {
  Motor_Move(-speed, -speed, speed, speed);   //turn left
  delay(1000);
  server.send(200, "text/plain", "Turning Left");
}

void handleRight() {
   Motor_Move(speed, speed, -speed, -speed);   //turn right
  delay(1000);
  server.send(200, "text/plain", "Turning Right");
}

void handleStop() {
  Motor_Move(0, 0, 0, 0);                 //stop
  delay(1000);
  server.send(200, "text/plain", "Stopped");
}



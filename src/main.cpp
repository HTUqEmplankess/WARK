// *WARK* : Smart Contactless Sleep Monitor : Sensor Node

#include <Arduino.h>
#include <Wire.h>
#include <NewPing.h>
#include <ArduinoJson.h>

#define RXD 16 // Receiver UART2
#define TXD 17 // Transmitter UART2

// Pin definitions
const int pirPin = 14;   // PIR Motion Sensor (Digital)
const int trigPin = 5;   // Ultrasonic Trig Pin
const int echoPin = 18;  // Ultrasonic Echo Pin
const int relayPin = 23; // Relay Pin for controlling 12V LED

// LED
int blynk_mode = 1;
int blynk_led = 0;

// PIR
int motionDetected = 0;
const int pirTime = 1000;

// Ultrasonic setup
NewPing sonar(trigPin, echoPin, 450);

void setup()
{
  // Serial monitor
  Serial.begin(115200);

  // UART2
  Serial2.begin(115200, SERIAL_8N1, RXD, TXD);

  // PIR Motion Sensor setup
  pinMode(pirPin, INPUT);

  // Ultrasonic OUT Pin setup
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);

  // Relay setup
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);

  delay(2000);
  digitalWrite(relayPin, LOW);
}

// ! Recommended to TEST 1 BY 1 device - uncomment each part of the loop

void loop()
{
  // Collect JsonData
  StaticJsonDocument<256> jsonData;
  StaticJsonDocument<256> ledJsonData;

  // 6. Ultrasonic and Pir Sensor: TEST Pass
  // lcd.clear();
  delay(pirTime);
  double distance = sonar.ping_cm(); // Calculate distance in cm
  motionDetected = digitalRead(pirPin);

  Serial.print("> ");
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm, ");

  Serial.print("> HumanDetect: ");

  String humanDetection = "None";

  if (Serial2.available() > 0)
  {
    String stringData = Serial2.readString();

    Serial.println(stringData);

    // Deserialization to JSON
    DeserializationError error = deserializeJson(ledJsonData, stringData);
    blynk_mode = ledJsonData["Blynk_MODE"];
    blynk_led = ledJsonData["Blynk_LED"];
    Serial.printf("\t\tBlynk_MODE:%d,Blynk_LED:%d\n", blynk_mode, blynk_led);
  }

  if (blynk_mode == 1)
  {
    // Auto Detect
    if (motionDetected == HIGH && ((distance > 0.0) && (distance < 200.0)))
    {
      humanDetection = "Detected";
      Serial.println("Detected");
      digitalWrite(relayPin, HIGH); // Turn ON relay (12V LED ON)
    }
    else
    {
      humanDetection = "None";
      Serial.println("None-Detected");
      digitalWrite(relayPin, LOW); // Turn OFF relay (12V LED OFF)
    }
  }
  else
  {
    // Manual
    if (motionDetected == HIGH && ((distance > 0.0) && (distance < 200.0)))
    {
      humanDetection = "Detected";
    }
    else{
      humanDetection = "None-Detected";
    }
    if (blynk_led == 1)
    {
      
      Serial.println("LED ON");
      digitalWrite(relayPin, HIGH); // Turn ON relay (12V LED ON)
    }
    else
    {
      Serial.println("LED OFF");
      digitalWrite(relayPin, LOW); // Turn OFF relay (12V LED OFF)
    }
  }

  jsonData["humanDetection"] = humanDetection;
  String output;
  serializeJson(jsonData, output);
  // Send JSON to another esp32
  Serial2.println(output);
  Serial.println("Sented");
}

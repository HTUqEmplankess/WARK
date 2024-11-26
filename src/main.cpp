// *WARK* : Smart Contactless Sleep Monitor : Sensor Node

#include <Arduino.h>
#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>

// Pin definitions
const int micPin = 36;       // KY-037 Microphone (Analog)
const int pirPin = 14;       // PIR Motion Sensor (Digital)
const int dhtPin = 15;       // DHT22 Data Pin
const int trigPin = 5;       // Ultrasonic Trig Pin 
const int echoPin = 18;      // Ultrasonic Echo Pin 
const int relayPin = 23;     // Relay Pin for controlling 12V LED
const int luxAnalogPin = 39; // Analog output of BH1750 (AOOR)
const int i2cSDA = 21;          // I2C SDA
const int i2cSCL = 22;          // I2C SCL

// PIR
int motionDetected = 0;
const int pirTime = 1000;

// DHT22 setup
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE);

// BH1750 setup (I2C)
BH1750 lightMeter;

// Ultrasonic setup
NewPing sonar (trigPin, echoPin, 450);

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust address if needed

void setup() {
  // Serial monitor
  Serial.begin(115200);

  // Microphone setup
  pinMode(micPin, INPUT);

  // PIR Motion Sensor setup
  pinMode(pirPin, INPUT);

  // Ultrasonic OUT Pin setup
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);


  // Relay setup
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);  

  // DHT22 setup
  dht.begin();

  // BH1750 Light Sensor setup (I2C)
  Wire.begin(i2cSDA, i2cSCL);  // Initialize I2C on GPIO21 (i2cSDA) and GPIO22 (i2cSCL)
  if (!lightMeter.begin()) {
    Serial.println("BH1750 initialization failed!");
    lcd.print("BH1750 Error!");
  }

  // LCD setup
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows
  lcd.backlight();
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
  digitalWrite(relayPin, LOW);  


}

// ! Recommended to TEST 1 BY 1 device - uncomment each part of the loop 


void loop() {
  
  // // 1. KY-037 : TEST Pass (too weak / LCD is OK)
  // //  lcd.clear(); // Clear the LCD for the next message
  // int soundLevel = analogRead(micPin);
  // Serial.print("> Sound_Level: ");
  // Serial.println(soundLevel);
  // lcd.setCursor(0, 0);
  // lcd.print("Sound_Level:");
  // lcd.setCursor(0, 1);
  // lcd.print(soundLevel);
  

  // // 2. PIR Motion Sensor : TEST Pass (too weak / relay is OK)
  // // lcd.clear();
  // delay(pirTime);
  // motionDetected = digitalRead(pirPin);
  // Serial.print("> Motion: ");
  // lcd.setCursor(0, 0);
  // lcd.print("Motion:");
  // if (motionDetected == LOW ) {
  //   Serial.println("0");
  //   lcd.setCursor(0, 1);
  //   lcd.print("None");
  //   digitalWrite(relayPin, LOW);  // Turn OFF relay (12V LED OFF)
  // } else {
  //   Serial.println("1");
  //   lcd.setCursor(0, 1);
  //   lcd.print("Have");
  //   digitalWrite(relayPin, HIGH); // Turn ON relay (12V LED ON)
  // }

  // // 3. DHT22 Temperature and Humidity : TEST Pass
  // // lcd.clear();
  // float temp = dht.readTemperature();
  // float hum = dht.readHumidity();
  // if (isnan(temp) || isnan(hum)) {
  //   Serial.println("Failed to read from DHT sensor!");
  //   lcd.setCursor(0, 0);
  //   lcd.print("DHT22 Error");
  // } else {
  //   Serial.print("> Temp: ");
  //   Serial.print(temp);
  //   Serial.print(" °C, Hum: ");
  //   Serial.print(hum);
  //   Serial.println(" %");
  //   lcd.setCursor(0, 0);
  //   lcd.print("Temp:");
  //   lcd.print(temp);
  //   lcd.print("C");
  //   lcd.setCursor(0, 1);
  //   lcd.print("Hum:");
  //   lcd.print(hum);
  //   lcd.print("%");
  // }

  // // 4. BH1750 Light Sensor : TEST Fail [CANNOT FIND IC] and [Something Wrong With AOOR]
  // float lux = lightMeter.readLightLevel();
  // Serial.print("> ");
  // Serial.print("LightC: ");
  // Serial.print(lux);
  // Serial.print(" lx, ");
  // lcd.setCursor(0, 0);
  // lcd.print("LightC:");
  // lcd.print(lux);
  // lcd.print(" lx");
  // int luxAnalog = analogRead(luxAnalogPin);
  // Serial.print("LightA: ");
  // Serial.print(luxAnalog);
  // Serial.println(" V");
  // lcd.setCursor(0, 1);
  // lcd.print("LightA:");
  // lcd.print(luxAnalog);

  // 5. Ultrasonic Sensor: TEST Pass (2cm - 200cm - 450cm for far range the obj should be big too.)
  // lcd.clear();
  // float distance = sonar.ping_cm() ; // Calculate distance in cm
  
  // Serial.print("> ");
  // Serial.print("Distance: ");
  // Serial.print(distance);
  // Serial.println(" cm, ");
  
  // lcd.setCursor(0, 0);
  // lcd.print("Dist:");
  // lcd.print(distance);
  // lcd.setCursor(12, 0);
  // lcd.print(" cm");  
  
}

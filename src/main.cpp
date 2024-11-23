#include "DHT.h"
#include <Wire.h>
#include <BH1750.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
const int micPin = 36;       // KY-037 Microphone (Analog)
const int pirPin = 14;       // PIR Motion Sensor (Digital)
const int dhtPin = 15;       // DHT22 Data Pin
// const int trigPin = 5;    // Ultrasonic Trig Pin (commented out for now)
// const int echoPin = 18;   // Ultrasonic Echo Pin (commented out for now)
const int relayPin = 19;     // Relay Pin for controlling 12V LED
const int luxAnalogPin = 39; // Analog output of BH1750 (AOOR)
const int lcdSDA = 21;       // I2C SDA
const int lcdSCL = 22;       // I2C SCL

// DHT22 setup
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE);

// BH1750 setup (I2C)
BH1750 lightMeter;

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust address if needed

void setup() {
  // Serial monitor
  Serial.begin(115200);

  // Microphone setup
  pinMode(micPin, INPUT);

  // PIR Motion Sensor setup
  pinMode(pirPin, INPUT);

  // Relay setup
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);  // Start with relay OFF

  // DHT22 setup
  dht.begin();

  // BH1750 Light Sensor setup (I2C)
  Wire.begin(lcdSDA, lcdSCL);  // Initialize I2C on GPIO21 (SDA) and GPIO22 (SCL)
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
}

// ! Recommended to TEST 1 BY 1 device - uncomment each part of the loop 


void loop() {
  // lcd.clear(); // Clear the LCD for the next message

  // // 1. KY-037 Microphone
  // int soundLevel = analogRead(micPin);
  // Serial.print("Sound Level: ");
  // Serial.println(soundLevel);
  // lcd.setCursor(0, 0);
  // lcd.print("Sound Level:");
  // lcd.setCursor(0, 1);
  // lcd.print(soundLevel);
  // delay(2000);

  // // 2. PIR Motion Sensor
  // lcd.clear();
  // int motionDetected = digitalRead(pirPin);
  // Serial.print("Motion: ");
  // lcd.setCursor(0, 0);
  // lcd.print("Motion:");
  // if (motionDetected) {
  //   Serial.println("Detected");
  //   lcd.setCursor(0, 1);
  //   lcd.print("Detected");
  //   digitalWrite(relayPin, HIGH); // Turn ON relay (12V LED ON)
  // } else {
  //   Serial.println("None");
  //   lcd.setCursor(0, 1);
  //   lcd.print("None");
  //   digitalWrite(relayPin, LOW);  // Turn OFF relay (12V LED OFF)
  // }
  // delay(2000);

  // // 3. DHT22 Temperature and Humidity
  // lcd.clear();
  // float temp = dht.readTemperature();
  // float hum = dht.readHumidity();
  // if (isnan(temp) || isnan(hum)) {
  //   Serial.println("Failed to read from DHT sensor!");
  //   lcd.setCursor(0, 0);
  //   lcd.print("DHT22 Error");
  // } else {
  //   Serial.print("Temp: ");
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
  // delay(2000);

  // // 4. BH1750 Light Sensor (I2C)
  // lcd.clear();
  // float lux = lightMeter.readLightLevel();
  // Serial.print("Light (I2C): ");
  // Serial.print(lux);
  // Serial.println(" lx");
  // lcd.setCursor(0, 0);
  // lcd.print("Light (I2C):");
  // lcd.setCursor(0, 1);
  // lcd.print(lux);
  // lcd.print(" lx");
  // delay(2000);

  // // 5. BH1750 Light Sensor (Analog)
  // lcd.clear();
  // int luxAnalogValue = analogRead(luxAnalogPin);
  // float luxAnalog = luxAnalogValue * (3.3 / 4095.0); // Assuming 3.3V ADC resolution
  // Serial.print("Light (Analog): ");
  // Serial.print(luxAnalog);
  // Serial.println(" V");
  // lcd.setCursor(0, 0);
  // lcd.print("Light (Analog):");
  // lcd.setCursor(0, 1);
  // lcd.print(luxAnalog);
  // lcd.print(" V");
  // delay(2000);

  // // 6. Ultrasonic Sensor (commented out)
  // lcd.clear();
  // digitalWrite(trigPin, LOW);
  // delayMicroseconds(2);
  // digitalWrite(trigPin, HIGH);
  // delayMicroseconds(10);
  // digitalWrite(trigPin, LOW);
  // long duration = pulseIn(echoPin, HIGH);
  // float distance = duration * 0.034 / 2;
  // Serial.print("Distance: ");
  // Serial.print(distance);
  // Serial.println(" cm");
  // lcd.setCursor(0, 0);
  // lcd.print("Distance:");
  // lcd.setCursor(0, 1);
  // lcd.print(distance);
  // lcd.print(" cm");
  // delay(2000);
  
}

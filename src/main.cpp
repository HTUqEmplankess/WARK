// *WARK* : Smart Contactless Sleep Monitor : Sensor Node

#include <Arduino.h>
#include "DHT.h"
#include <Wire.h>
#include <BH1750FVI.h>
#include <LiquidCrystal_I2C.h>
#include <NewPing.h>
#include <ArduinoJson.h>

#define RXD 16 // Receiver UART2
#define TXD 17 // Transmitter UART2

// Pin definitions
const int pirPin = 14;   // PIR Motion Sensor (Digital)
const int dhtPin = 15;   // DHT22 Data Pin
const int trigPin = 5;   // Ultrasonic Trig Pin
const int echoPin = 18;  // Ultrasonic Echo Pin
const int relayPin = 23; // Relay Pin for controlling 12V LED
const int i2cSDA = 21;   // I2C SDA
const int i2cSCL = 22;   // I2C SCL

// PIR
int motionDetected = 0;
const int pirTime = 1000;

// DHT22 setup
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE);

// BH1750 setup (I2C)
BH1750FVI lightMeter(BH1750_DEFAULT_I2CADDR, BH1750_ONE_TIME_HIGH_RES_MODE, BH1750_SENSITIVITY_DEFAULT, BH1750_ACCURACY_DEFAULT);

// Ultrasonic setup
NewPing sonar(trigPin, echoPin, 450);

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address if needed

void setup()
{
  // Serial monitor
  Serial.begin(115200);

  // UART2
  Serial2.begin(9600, SERIAL_8N1, RXD, TXD);

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
  Wire.begin(i2cSDA, i2cSCL); // Initialize I2C on GPIO21 (i2cSDA) and GPIO22 (i2cSCL)
  if (!lightMeter.begin())
  {
    Serial.println("BH1750 initialization failed!");
    lcd.print("BH1750 Error!");
  }
  /* change BH1750 settings on the fly */
  lightMeter.setCalibration(1.06);                           // call before "readLightLevel()", 1.06=white LED & artifical sun
  lightMeter.setSensitivity(1.00);                           // call before "readLightLevel()", 1.00=no optical filter in front of the sensor
  lightMeter.setResolution(BH1750_CONTINUOUS_HIGH_RES_MODE); // call before "readLightLevel()", continuous measurement with 1.00 lux resolution

  // LCD setup
  lcd.begin(16, 2); // Initialize LCD with 16 columns and 2 rows
  lcd.backlight();
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();
  digitalWrite(relayPin, LOW);
}

// ! Recommended to TEST 1 BY 1 device - uncomment each part of the loop

void loop()
{
  // Collect JsonData
  StaticJsonDocument<256> jsonData;

  // 3. DHT22 Temperature and Humidity : TEST Pass
  delay(pirTime);
  lcd.clear();
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  if (isnan(temp) || isnan(hum))
  {
    jsonData["Temperature"] = isnan(temp) ? NULL : temp;
    jsonData["Humidity"] = isnan(hum) ? NULL : hum;
    Serial.println("Failed to read from DHT sensor!");
    lcd.setCursor(0, 0);
    lcd.print("DHT22 Error");
  }
  else
  {
    jsonData["Temperature"] = temp;
    jsonData["Humidity"] = hum;
    Serial.print("> Temp: ");
    Serial.print(temp);
    Serial.print(" °C, Hum: ");
    Serial.print(hum);
    Serial.println(" %");
    lcd.setCursor(0, 0);
    lcd.print("Temp:");
    lcd.print(temp);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Hum:");
    lcd.print(hum);
    lcd.print("%");
  }

  // 4. BH1750 Light Sensor : TEST Pass (PLS Carefull with the PCB)
  delay(pirTime);
  lcd.clear();
  float lux = lightMeter.readLightLevel(); // Read light level in lux
  if (lux == BH1750_ERROR)
  {
    jsonData["lux"] = NULL;
    Serial.println("> LightC: ERROR");
    lcd.setCursor(0, 0);
    lcd.print("LightC: ERROR   "); // Clear any previous data
  }
  else
  {
    jsonData["lux"] = lux;
    Serial.print("> LightC: ");
    Serial.print(lux);
    Serial.println(" lx");

    lcd.setCursor(0, 0);
    lcd.print("LightC:");
    lcd.print(lux, 2); // Display with two decimal places
    lcd.setCursor(14, 0);
    lcd.print("lx");
  }

  // 6. Ultrasonic and Pir Sensor: TEST Pass
  // lcd.clear();
  delay(pirTime);
  double distance = sonar.ping_cm(); // Calculate distance in cm
  motionDetected = digitalRead(pirPin);

  Serial.print("> ");
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm, ");

  Serial.print("> Motion: ");
  // if (motionDetected == LOW)
  // {
  //   Serial.println("0");
  // }
  // else
  // {
  //   Serial.println("1");
  // }

  lcd.clear();
  String humanDetection = "None";

  if (motionDetected == HIGH && ((distance > 0.0) && (distance < 200.0)))
  {
    humanDetection = "Detected";
    Serial.println("Detected");
    lcd.print("Detected");
    digitalWrite(relayPin, HIGH); // Turn ON relay (12V LED ON)
  }
  else
  {
    humanDetection = "None";
    Serial.println("None-Detected");
    lcd.print("None-Detected");
    digitalWrite(relayPin, LOW); // Turn OFF relay (12V LED OFF)
  }
  jsonData["humanDetection"] = humanDetection;
  String output;
  serializeJson(jsonData, output);
  Serial.print("> json: ");
  Serial.print(output);
  Serial.print("\n ------------------------------ \n");
}

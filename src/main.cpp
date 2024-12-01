#define BLYNK_TEMPLATE_ID "TMPL6YgbGJxbo"
#define BLYNK_TEMPLATE_NAME "Sleep Monitor"
#define BLYNK_AUTH_TOKEN "vOFeGSR9qI3YG1k9KbTS2wjfJgCB42BQ"
#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <Wifi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <ArtronShop_LineNotify.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <BH1750FVI.h>

#define RX1 18 // Receiver UART1
#define TX1 19 // Transmitter UART1
#define RX2 16 // Receiver UART2
#define TX2 17 // Transmitter UART2

const int dhtPin = 15; // DHT22 Data Pin

// Timestamp of the last action
unsigned long lastTimeAwake = 0;
const unsigned long debounceAwake = 60 * 1000; // 10 minutes in milliseconds
unsigned long lastTimeSnore = 0;
const unsigned long debounceSnore = 60 * 1000; // 10 minutes in milliseconds

// DHT22 setup
#define DHTTYPE DHT22
DHT dht(dhtPin, DHTTYPE);

LiquidCrystal_I2C lcd(0x27, 16, 2);

// BH1750 setup (I2C)
BH1750FVI lightMeter(BH1750_DEFAULT_I2CADDR, BH1750_ONE_TIME_HIGH_RES_MODE, BH1750_SENSITIVITY_DEFAULT, BH1750_ACCURACY_DEFAULT);

// Network Credentials
const char *ssid = "HTUq";
const char *password = "22_07_66";

#define LINE_TOKEN "6xe7YFuAHTaJYAznKWJSmEtdHD9FGOAysoaONRVGop1"
#define GOOGLE_SCRIPT_URL "https://script.google.com/macros/s/AKfycbz9e3kvlAyHrM_jBuQ6PbXRxqf6YELR2A1f1_3aMNDgmSOwe79kb-hTjYt0eF3ZZs778w/exec"

// Variable Declaration
float temp;
float hum;
float lux;
String humanDetection = "";

String command = "Not_Snoring";
int score = -1;
String timeA = "-1";

void sendToCloud(float temp, float hum, float lux, String humanDetection, String snoring, int score)
{
  // Google Sheet Part
  HTTPClient http;

  // Create the URL with parameters
  String url = String(GOOGLE_SCRIPT_URL) + "?value1=" + String(temp) +
               "&value2=" + String(hum) +
               "&value3=" + String(lux) +
               "&value4=" + String(humanDetection) +
               "&value5=" + String(snoring) +
               "&value6=" + String(score);

  // Serial.println(url);
  http.begin(url);           // Initialize HTTP request
  int httpCode = http.GET(); // Send GET request

  if (httpCode == 200)
  {
    Serial.printf("Data sent to Google Sheets. HTTP Response: %d\n", httpCode);
  }
  else
  {
    Serial.printf("Error sending to Google Sheets: %d : %s\n", httpCode, http.errorToString(httpCode).c_str());
  }

  http.end(); // Close connection

  // Blynk Part
  Blynk.virtualWrite(V1, snoring);
  Blynk.virtualWrite(V2, humanDetection);
  Blynk.virtualWrite(V3, temp);
  Blynk.virtualWrite(V4, hum);
  Blynk.virtualWrite(V5, lux);
}

void updateLCD(float temp, float hum, float lux, String humanDetection)
{
  static float lastTemp = -1;
  static float lastHum = -1;
  static float lastLux = -1;
  static String lastHumanDetection = "";

  if (temp != lastTemp || hum != lastHum)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temp);
    lcd.print(" C");
    lcd.setCursor(0, 1);
    lcd.print("Hum: ");
    lcd.print(hum);
    lcd.print(" %");
    lastTemp = temp;
    lastHum = hum;
    delay(1000);
  }

  if (lux != lastLux || humanDetection != lastHumanDetection)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Lux: ");
    lcd.print(lux);
    lcd.print(" lx");
    lcd.setCursor(0, 1);
    lcd.print("Human: ");
    lcd.print(humanDetection);
    lastLux = lux;
    lastHumanDetection = humanDetection;
    delay(1000);
  }
}

void setup()
{
  // Serial monitor
  Serial.begin(115200);

  // Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  // UART2
  Serial1.begin(115200, SERIAL_8N1, RX1, TX1);
  Serial2.begin(9600, SERIAL_8N1, RX2, TX2);

  // DHT22 setup
  dht.begin();

  // BH1750 Light Sensor setup (I2C)
  if (!lightMeter.begin())
  {
    Serial.println("BH1750 initialization failed!");
    lcd.print("BH1750 Error!");
  }
  /* change BH1750 settings on the fly */
  lightMeter.setCalibration(1.06);                           // call before "readLightLevel()", 1.06=white LED & artifical sun
  lightMeter.setSensitivity(1.00);                           // call before "readLightLevel()", 1.00=no optical filter in front of the sensor
  lightMeter.setResolution(BH1750_CONTINUOUS_HIGH_RES_MODE); // call before "readLightLevel()", continuous measurement with 1.00 lux resolution

  // LCD
  Wire.begin(21, 22);
  lcd.begin(16, 2); // Initialize LCD with 16 columns and 2 rows
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");

  // Wifi Station mode
  WiFi.mode(WIFI_STA);

  // Wifi ID & Password setup
  WiFi.begin(ssid, password);

  // Wifi Status Check
  Serial.print("Connecting To WiFi Network");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nConnected To The WiFi Network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());

  LINE.begin(LINE_TOKEN);

  lcd.clear();
}

void loop()
{
  // Blynk.run();
  // 3. DHT22 Temperature and Humidity : TEST Pass
  float tempN = dht.readTemperature();
  float humN = dht.readHumidity();
  float luxN = lightMeter.readLightLevel(); // Read light level in lux
  if (luxN == BH1750_ERROR)
  {
    Serial.println("> LightC: ERROR");
    lcd.setCursor(0, 0);
    lcd.print("LightC: ERROR   "); // Clear any previous data
  }

  // Receive JSON from Node Sensor Sender
  if (Serial2.available() > 0)
  {
    int ch = 0;
    String jsonString = Serial2.readStringUntil('\n');
    StaticJsonDocument<256> jsonData;

    // Deserialization to JSON
    DeserializationError error = deserializeJson(jsonData, jsonString);

    if (!error)
    {
      String humanDetectionN = jsonData["humanDetection"];

      // Show Data on Serial
      Serial.print("Temperature: ");
      Serial.print(tempN);
      Serial.println("ํ °C");
      Serial.print("Humidity: ");
      Serial.print(humN);
      Serial.println("ํ %");
      Serial.print("Light: ");
      Serial.print(luxN);
      Serial.println("ํ lux");
      Serial.print("humanDetection: ");
      Serial.println(humanDetectionN);

      if (tempN != temp)
      {
        temp = tempN;
        ch = 1;
      }
      if (humN != hum)
      {
        hum = humN;
        ch = 1;
      }
      if (luxN != lux)
      {
        lux = luxN;
        ch = 1;
      }
      if (humanDetectionN != humanDetection)
      {
        humanDetection = humanDetectionN;
        ch = 1;
      }
      
      updateLCD(temp, hum, lux, humanDetectionN);
      if (humanDetectionN == "Detected")
      {
        // debounce line
        unsigned long currentTime = millis(); // Get the current time in milliseconds
        if (currentTime - lastTimeAwake >= debounceAwake)
        {
          LINE.send("You are awaking!");
          lastTimeAwake = currentTime; // Update the last action time
        }
      }

      if (ch)
      {
        sendToCloud(temp, hum, lux, humanDetection, command, score);
      }
    }
    lcd.clear();
  }

  // Receive JSON from Node Sensor Sender
  if (Serial1.available() > 0)
  {
    lcd.print("Snoring Detect");
    String jsonString = Serial1.readStringUntil('\n');
    StaticJsonDocument<256> jsonData;

    // Deserialization to JSON
    DeserializationError error = deserializeJson(jsonData, jsonString);

    if (!error)
    {
      command = jsonData["command"].as<String>();
      score = jsonData["score"];
      timeA = jsonData["time"].as<String>();

      // Show Data on Serial
      Serial.print("You are ");
      Serial.print(command);
      Serial.print(" score ");
      Serial.println(score);

      // debounce line
      unsigned long currentTime = millis();
      if (currentTime - lastTimeSnore >= debounceSnore)
      {
        LINE.send("You are snoring!");
        lastTimeSnore = currentTime; // Update the last action time
      }
    }

    sendToCloud(temp, hum, lux, humanDetection, command, score);

    command = "not_snoring";
    score = -1;
    timeA = "-1";
  }
}
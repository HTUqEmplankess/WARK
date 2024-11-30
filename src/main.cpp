#include <Arduino.h>
#include <Wifi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <ArtronShop_LineNotify.h>

#define RXD 16 // Receiver UART2
#define TXD 17 // Transmitter UART2

// Network Credentials
const char *ssid = "HTUq";
const char *password = "22_07_66";

#define LINE_TOKEN "6xe7YFuAHTaJYAznKWJSmEtdHD9FGOAysoaONRVGop1" 
#define GOOGLE_SCRIPT_URL "https://script.google.com/macros/s/AKfycbyJxgloJnvfXQhBN3G4k1gPBY7_imxUlC2YwHSquCKPJJzk9i53HVcoPNUvTVdgwEmrGA/exec"

// Variable Declaration
float temp;
float hum;
float lux;
String humanDetection = "";

String command = "Not Snoring";
int score = -1;
String timeA = "-1" ;


void sendToGoogleSheets(float temp, float hum, float lux, String humanDetection,String snoring,int score) {
  HTTPClient http;

  // Create the URL with parameters
  String url = String(GOOGLE_SCRIPT_URL) + "?value1=" + String(temp) +
               "&value2=" + String(hum) +
               "&value3=" + String(lux) +
               "&value4=" + String(humanDetection)+
               "&value5=" + String(snoring)+
               "&value6=" + String(score);

  http.begin(url);  // Initialize HTTP request
  int httpCode = http.GET();  // Send GET request

  if (httpCode == 200) {
    Serial.printf("Data sent to Google Sheets. HTTP Response: %d\n", httpCode);
  } else {
    Serial.printf("Error sending to Google Sheets: %d : %s\n", httpCode, http.errorToString(httpCode).c_str());
  }

  http.end();  // Close connection
}

void setup()
{
  // Serial monitor
  Serial.begin(115200);

  // UART2
  Serial2.begin(9600, SERIAL_8N1, RXD, TXD);

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
}

void loop()
{
  // Receive JSON from Node Sensor Sender
  // if (Serial2.available() > 0)
  // {
  //   String jsonString = Serial2.readStringUntil('\n');
  //   StaticJsonDocument<256> jsonData;

  //   // Deserialization to JSON
  //   DeserializationError error = deserializeJson(jsonData, jsonString);

  //   if (!error)
  //   {
  //     float temp = jsonData["temperature"];
  //     float hum = jsonData["humidity"];
  //     float lux = jsonData["lux"];
  //     String humanDetection = jsonData["humanDetection"];

  //     // Show Data on Serial
  //     Serial.print("Temperature: ");
  //     Serial.print(temp);
  //     Serial.println("ํ °C");
  //     Serial.print("Humidity: ");
  //     Serial.print(hum);
  //     Serial.println("ํ %");
  //     Serial.print("Light: ");
  //     Serial.print(lux);
  //     Serial.println("ํ lux");
  //     Serial.print("humanDetection: ");
  //     Serial.println(humanDetection);
  //     if(humanDetection=="Detected"){
  //       LINE.send("You are awaking!");
  //     }
  // sendToGoogleSheets(temp, hum, lux, humanDetection, command, score);
  //   }

    
  // }

  // Receive JSON from Node Sensor Sender
  if (Serial2.available() > 0)
  {
    String jsonString = Serial2.readStringUntil('\n');
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
      LINE.send("You are snoring!");
    }
  sendToGoogleSheets(temp, hum, lux, humanDetection, command, score);

    command = "Not Snoring";
    score = -1;
    timeA = "-1" ;
  }

}
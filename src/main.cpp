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

#define LINE_TOKEN "6xe7YFuAHTaJYAznKWJSmEtdHD9FGOAysoaONRVGop1" // LINE Token


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
      String command = jsonData["command"];
      int score = jsonData["score"];
      String time = jsonData["time"];

      // Show Data on Serial
      Serial.print("You are ");
      Serial.print(command);
      Serial.print(" score ");
      Serial.println(score);
      LINE.send("You are snoring!");
    }
  }
}
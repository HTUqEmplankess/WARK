#include <Arduino.h>
#include <Wifi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

#define RXD 16 // Receiver UART2
#define TXD 17 // Transmitter UART2

// Network Credentials
const char *ssid = "HTUq";
const char *password = "22_07_66";

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
}

void loop()
{
  // Receive JSON from Sender
  if (Serial2.available() > 0)
  {
    String input = Serial2.readString();
    Serial.println(input);
  }
}
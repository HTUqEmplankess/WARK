// *WARK* : Smart Contactless Sleep Monitor : Snore Detection Integration

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pin definitions
const int relayPin = 23;     // Relay Pin for controlling 12V LED
const int i2cSDA = 21;          // I2C SDA
const int i2cSCL = 22;          // I2C SCL

// LCD setup
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Adjust address if needed

void setup() {
  // Serial monitor
  Serial.begin(115200);
  Serial.println("Hi");

  // LCD setup
  lcd.begin(16, 2);  // Initialize LCD with 16 columns and 2 rows
  lcd.backlight();
  lcd.print("Initializing...");
  delay(2000);
  lcd.clear();

  digitalWrite(relayPin, LOW);  

}

void loop() {
  lcd.print("Done...........");
}

#include "Arduino.h"

// Define constants for pin numbers
const int buttonPin = 2;  // Pin connected to the button
const int ledPin = 15;    // Pin connected to the LED

// Variables to track LED and button states
int ledState = HIGH;        // Initial LED state (on)
int buttonState;            // Current button state
int lastButtonState = LOW;  // Previous button state

// Variables for debouncing
unsigned long lastDebounceTime = 0;  // Tracks last debounce time
unsigned long debounceDelay = 50;   // Debounce delay in milliseconds

void setup() {
  // Set pin modes
  pinMode(buttonPin, INPUT);  // Configure button pin as input
  pinMode(ledPin, OUTPUT);    // Configure LED pin as output

  // Initialize LED state
  digitalWrite(ledPin, ledState);  // Set initial LED state
}

void loop() {
  // Read the current state of the button
  int reading = digitalRead(buttonPin);

  // Check for changes in button state
  if (reading != lastButtonState) {
    // Reset debounce timer if the state changed
    lastDebounceTime = millis();
  }

  // Check if debounce delay has passed
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // Update button state if it has stabilized
    if (reading != buttonState) {
      buttonState = reading;

      // Toggle LED state only on button press (HIGH)
      if (buttonState == HIGH) {
        ledState = !ledState;  // Invert LED state
      }
    }
  }

  // Update the LED with the current state
  digitalWrite(ledPin, ledState);

  // Save the current reading as the last state for the next loop
  lastButtonState = reading;
}

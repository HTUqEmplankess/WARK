/*
  Debounce

  Each time the input pin goes from LOW to HIGH (e.g. because of a push-button
  press), the output pin is toggled from LOW to HIGH or HIGH to LOW. There's a
  minimum delay between toggles to debounce the circuit (i.e. to ignore noise).

  The circuit:
  - LED attached from pin 13 to ground through 220 ohm resistor
  - pushbutton attached from pin 2 to +5V
  - 10 kilohm resistor attached from pin 2 to ground

  - Note: On most Arduino boards, there is already an LED on the board connected
    to pin 13, so you don't need any extra components for this example.

  created 21 Nov 2006
  by David A. Mellis
  modified 30 Aug 2011
  by Limor Fried
  modified 28 Dec 2012
  by Mike Walters
  modified 30 Aug 2016
  by Arturo Guadalupi

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/Debounce
*/
# include "Arduino.h"

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;  // ตั้งตัวแปรคงที่ buttonPin เป็นประเภท int ให้เท่ากับ 2
const int ledPin = 15;    // ตั้งตัวแปรคงที่ ledPin เป็นประเภท int ให้เท่ากับ 15

// Variables will change:
int ledState = HIGH;        // ตั้งตัวแปร ledState เป็นประเภท int ให้เท่ากับ 1 หรือ HIGH
int buttonState;            // ตั้งตัวแปร buttonState เป็นประเภท int ให้เท่ากับ 0
int lastButtonState = LOW;  // ตั้งตัวแปร buttonState เป็นประเภท int ให้เท่ากับ 0 หรือ LOW

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // ตั้งตัวแปร lastDebounceTime เป็นประเภท unsigned long ให้เท่ากับ 0 หรือ LOW
unsigned long debounceDelay = 50;    // ตั้งตัวแปร debounceDelay เป็นประเภท unsigned long ให้เท่ากับ 50

void setup() { // เป็น ฟังก์ชัน ที่จะเรียกใช้เพียง 1 ครั้ง
  pinMode(buttonPin, INPUT); // เป็นการกำหนด PIN หมายเลขที่ 2 ให้เป็นประเภท INPUT
  pinMode(ledPin, OUTPUT); // เป็นการกำหนด PIN หมายเลขที่ 15 ให้เป็นประเภท OUTPUT

  // set initial LED state
  digitalWrite(ledPin, ledState); //เป็นฟังก์ชันที่ใช้ในการเขียนสถานะของขา PIN 15 ให้ค่าเท่ากับ ledState
}

void loop() { // เป็น ฟังก์ชัน ที่จะเรียกใช้ตลอด
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin); // ให้ reading เป็นค่าที่ได้จากฟังก์ชันที่ใช้ในการอ่านสถานะของขา PIN 2

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) { //ถ้า reading ไม่มีค่าเท่ากับ lastButtonState
    // reset the debouncing timer
    lastDebounceTime = millis(); //ให้ lastDebounceTime = เวลาที่ผ่านไปหลังจากเริ่มโปรแกรม(หน่วยมิลิวินาที)
  }

  if ((millis() - lastDebounceTime) > debounceDelay) { //ถ้าเวลาที่ผ่านไปหลังจากเริ่มโปรแกรม - lastDebounceTime มีค่ามากกว่า debounceDelay 
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {//ถ้า reading มีค่าไม่เท่ากับ buttonState
      buttonState = reading; // ให้ buttonState = reading

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {//ถ้า reading มีค่าเท่ากับ HIGH
        ledState = !ledState; // ให้ ledState = นิเสธ ledState
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, ledState); // เรียกฟังก์ชันที่ใช้ในการเขียนสถานะให้เป็นค่า ledState ในขาของ PIN 15

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading; //ให้ค่า lastButtonState = reading
}

/*
Date: May 17th 2024 
Description: test all inputs and outputs on the breadboard version: 
- 2 buttons
- 2 LEDs 

*/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


const int blueLED = 6; // using this instead of buzzer for now
const int redLED = 3; // using this instead of vibration motor for now 

const int okButton = 7; 
const int backButton = 8; 

const int SCREEN_WIDTH = 128; // OLED display width, in pixels
const int SCREEN_HEIGHT = 64; // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  Serial.begin(9600);
  pinMode(blueLED, OUTPUT); 
  pinMode(redLED, OUTPUT); 
  pinMode(okButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP); 

}

void loop() {

  // if (digitalRead(okButton) == LOW){
  //   Serial.println("ok===============================================");
  // }
  // if (digitalRead(backButton) == LOW){
  //   Serial.println("back----");
  // }
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  // float time = millis(); 
  // display.println(millis()/1000.0, 2);
  // display.display();

  display.println("Hello World"); 
  display.display(); 

}

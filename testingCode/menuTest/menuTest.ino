/*
Date: June 25th 2024 
Description: test the menu and interaction with the buttons 

*/


#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();

}

void loop() {



  // first row 
  display.setTextSize(2);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);            

  display.print("mode ");
  display.setCursor(90,0);
  // display.print("stp");
  display.print("tmr");

  
  // second row 
  display.setCursor(0,32); 
  display.print("output");
  display.setCursor(90,32);
  // display.print("bzz");
  // display.print("rng");
  display.print("amb");
  display.display();

}
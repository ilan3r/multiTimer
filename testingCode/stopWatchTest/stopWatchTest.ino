/*
Date: June 25th 2024 
Description: test stopwatch display on OLED, format properly with min:seconds.milliseconds 

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
  display.display();


}

void loop() {

  display.clearDisplay();


  // at this size, assuming that we won't reach 10 minutes 
  display.setTextSize(3);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);             // Start at top-left corner


  float timeSec = millis()/1000.0; 
  int min = 0; 

  while (timeSec >= 60.0)
  {
    timeSec = timeSec - 60.0; 
    min++; 
  }
  
  if (min > 0)
  {
    display.print(min);
    display.print(":");

    if (timeSec < 10.0)
    {
      display.print("0");
      display.print(timeSec); 
      display.display();
    }
    else
    {
      display.print(timeSec);
      display.display();
    }
  }
  else{
    if (timeSec < 10.0)
    {
      display.print("0");
      display.print(timeSec); 
      display.display();
    }

    else
    {
      display.println(millis()/1000.0, 2);
      display.display();

    }
  }
  delay(10);

}

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


// pin variables 
const int blueLED = 3; // using this instead of buzzer for now
const int redLED = 6; // using this instead of vibration motor for now 
const int okButton = 7; 
const int backButton = 8; 


// menu variables 
String currentOutput = "dual";
String currentMode = "timer";
bool cursorInRow1 = true;  // start cursor in first row


// button variables 
long unsigned currentTime = 0; 
const int debounceDelay = 10; 
long okLastDetection;
int okButtonState = 1; // initialize these to unpressed 
int okLastButtonState = 1;
bool okPressed = 1;
unsigned long okStartPressed = 0; 
unsigned long okEndPressed = 0;
unsigned long okHoldTime = 0; 
unsigned long okIdleTime = 0;
long backLastDetection;
int backButtonState = 1;  // initialize these to unpressed 
int backLastButtonState = 1;
bool backPressed = 1;
unsigned long backStartPressed = 0; 
unsigned long backEndPressed = 0;
unsigned long backHoldTime = 0; 
unsigned long backIdleTime = 0;

  void buttonState();
  void setupMenu(String modeType, String outputType);
  void toggleOutputType(); 
  void toggleMode();
  // void handle_stopwatch_paused();
  // void handle_stopwatch_run(); 
  // void handle_stopwatch_primed(); 
  // void handle_menu(); 

void setup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();
  pinMode(okButton, INPUT_PULLUP); 
  pinMode(backButton, INPUT_PULLUP); 
  pinMode(blueLED, OUTPUT); 
  pinMode(redLED, OUTPUT); 

}

void loop() {
  setupMenu(currentMode, currentOutput);
  buttonState();



  if (okButtonState == 0){
    if (cursorInRow1){
      toggleMode();
    }
    else if (!cursorInRow1){
      toggleOutputType();
    }
  

  }
  if (backButtonState == 0){
    cursorInRow1 = !(cursorInRow1);
  }
  



}

void setupMenu(String modeType, String outputType){
  
  display.clearDisplay(); 

  // first row 
  display.setTextSize(2);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);            

  display.print("mode ");
  display.setCursor(90,0);


  if (modeType == "timer"){
    display.print("tmr");
  }
  else if (modeType == "stopwatch"){
    display.print("stp");
  }
  
  // second row 
  display.setCursor(0,32); 
  display.print("output");
  display.setCursor(90,32);

  if (outputType == "haptic"){
    display.print("vib");
  }
  else if (outputType == "sound"){
    display.print("bzz");
  }
  else if (outputType == "dual"){
    display.print("amb");
  }

  display.display(); 
}

void toggleMode(){
  if (currentMode == "timer"){
    currentMode = "stopwatch";
  }
  else if (currentMode == "stopwatch"){
    currentMode = "timer";
  }

}

void toggleOutputType(){

  // cycle haptic --> sound --> dual --> 
  if (currentOutput == "haptic"){
    currentOutput = "sound";
  }
  else if (currentOutput == "sound"){
    currentOutput = "dual";
  }
  else if (currentOutput == "dual"){
    currentOutput = "haptic";
  }

}


void buttonState(){
  int okReading = digitalRead(okButton); 
  int backReading = digitalRead(backButton);

  if (okButtonState != okLastButtonState){
    okLastDetection = millis();
  }

  if (backButtonState != backLastButtonState){
    backLastDetection = millis();
  }


  // only accept the change in button state after debounce time
  if ( (millis() - okLastDetection) > debounceDelay ){
    okButtonState = okReading; 
  }
  // only accept the change in button state after debounce time
  if ( (millis() - backLastDetection) > debounceDelay ){
    backButtonState = backReading; 
  }



  // if the button is pressed or released
  if (okButtonState != okLastButtonState)
  {

    // if the button is just pressed 
    if (okButtonState == 0)
    {
      okStartPressed = millis(); 
      // if (okButtonState == 0 & okPressed == 1)
      // {
      //   Serial.println("======just pressed ok =======");
      // }
      okPressed = 0; 

    }


    // if the button is just released 
    else if (okButtonState == 1)
    {

      okEndPressed = millis();
      okHoldTime = okEndPressed - okStartPressed; 
      // if (okButtonState == 1 && okPressed == 0)
      // {
      //   Serial.print("---just released ok ----: holdtime: ");
      //   Serial.println(okHoldTime);
      // }

      okPressed = 1;
    }


  }

  // if the back button is pressed or released
  if (backButtonState != backLastButtonState)
  {

    // if the button is just pressed 
    if (backButtonState == 0)
    {
      backStartPressed = millis(); 
      if (backButtonState == 0 & backPressed == 1)
      // {
      //   Serial.println("======just pressed back =======");
      // }
      backPressed = 0; 

    }


    // if the button is just released 
    else if (backButtonState == 1)
    {

      backEndPressed = millis();
      backHoldTime = backEndPressed - backStartPressed; 
      if (backButtonState == 1 && backPressed == 0)
      // {
      //   Serial.print("---just released back ----: holdtime: ");
      //   Serial.println(backHoldTime);
      // }

      backPressed = 1;
    }


  }


  okLastButtonState = okReading; 
  backLastButtonState = backReading; 



}







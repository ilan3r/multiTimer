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

enum State {timer_run, timer_paused};
State currentState = timer_paused;



// timer variables 

float timerSec = 0; 
float timer1Sec = 10.0; 
float timer2Sec = 5.0;
unsigned long timerDelay = 0; 

int timerTracker = 1; // bounces between 1 and 2



// timer function prototypes
void buttonState();
void pauseTimer();
void runTimer();
void handle_timer_run();
void handle_timer_paused();


// button variables
  const int okButton = 7; 
  const int backButton = 8; 
  const bool serialEnabled = 1; 
  long unsigned currentTime = 0; 
  const int debounceDelay = 10; 
  long okLastDetection = 0;
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
  bool okJustChanged = 0; 
  bool backJustChanged = 0;





void setup() {

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.display();


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
      if (okButtonState == 0 && okPressed == 1)
      {
        if (serialEnabled) Serial.println("======just pressed ok =======");
      }
      okPressed = 0;
      okJustChanged = 1; 

    }


    // if the button is just released 
    else if (okButtonState == 1)
    {

      okEndPressed = millis();
      okHoldTime = okEndPressed - okStartPressed; 
      if (okButtonState == 1 && okPressed == 0)
      {
        if (serialEnabled) {
          Serial.print("---just released ok ----: holdtime: ");
          Serial.println(okHoldTime);
        }
      }
      okJustChanged = 1; 
      okPressed = 1;
    }


  }
  else {
    okJustChanged = 0; 
  }

  // if the back button is pressed or released
  if (backButtonState != backLastButtonState)
  {

    // if the button is just pressed 
    if (backButtonState == 0)
    {
      backStartPressed = millis(); 
      if (backButtonState == 0 && backPressed == 1)
      {
        if (serialEnabled) Serial.println("======just pressed back =======");
      }
      backJustChanged = 1; 
      backPressed = 0; 

    }


    // if the button is just released 
    else if (backButtonState == 1)
    {

      backEndPressed = millis();
      backHoldTime = backEndPressed - backStartPressed; 
      if (backButtonState == 1 && backPressed == 0)
      {
        if (serialEnabled){
          Serial.print("---just released back ----: holdtime: ");
          Serial.println(backHoldTime);
        }

      }

      backJustChanged = 1;
      backPressed = 1;
    }

    else {
      okJustChanged = 0; 
      backJustChanged = 0; 
    }


  }

  else {
    backJustChanged = 0; 
  }


  okLastButtonState = okReading; 
  backLastButtonState = backReading; 



}

void loop() {

  switch (currentState) {
    case timer_run:
      handle_timer_run();
      break;

    case timer_paused:
      handle_timer_paused(); 
      break;
  }


}


void pauseTimer(){
  display.clearDisplay();
  display.setTextSize(3);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0); 

  if (timerTracker == 1){
    display.print(timer1Sec - timerSec);
    display.display();
  }
  else if (timerTracker == 2){
    display.print(timer2Sec - timerSec);
    display.display();
  }

  
}


void runTimer(){
  display.clearDisplay();
  display.setTextSize(3);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);             // Start at top-left corner

  timerSec = (millis() - timerDelay)/1000.0;

  if (timerTracker == 1){
    if (timer1Sec- timerSec > 0){
      display.print(timer1Sec - timerSec);
      display.display();
    }
    else {
      timerTracker = 2;
      timerDelay = millis();
    }
  }

  else if (timerTracker == 2){
    if (timer2Sec - timerSec > 0){
      display.print(timer2Sec - timerSec);
      display.display();
    }
    else {
      timerTracker = 1; 
      timerDelay = millis();
    }


  }


}

void handle_timer_run(){
  buttonState(); 
  runTimer();

  if ((okButtonState == 0 and okJustChanged) || (backButtonState == 0 and backJustChanged)){
    currentState = timer_paused;
  }

}


void handle_timer_paused(){
  buttonState(); 
  pauseTimer(); 

  if (okButtonState == 0 and okJustChanged){
    currentState = timer_run;
    timerDelay = millis() - (timerSec*1000.0);
  }

  if (backButtonState == 0 and backJustChanged){
    timerSec = 0;
    timerDelay = millis();
  }


}

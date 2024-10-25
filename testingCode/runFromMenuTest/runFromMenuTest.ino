/*
code for testing the menu functionality, and actually starting the stopwatch/timer from the menu

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



enum State {stopwatch_primed, stopwatch_paused, stopwatch_run, timer_run, menu};
State currentState = menu;
int randomStart = 0; 
unsigned int startRandomWait = 0;




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


// function prototypes
  void buttonState();
  void setupMenu(String modeType, String outputType);
  void toggleOutputType(); 
  void toggleMode();
  void handle_stopwatch_paused();
  void handle_stopwatch_run(); 
  void handle_stopwatch_primed(); 
  void handle_menu();
  void handle_timer_run();



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

  switch (currentState) {
    case menu:
      handle_menu();
      break;
    case timer_run:
      handle_timer_run(); 
      break; 
    case stopwatch_paused: 
      handle_stopwatch_paused();
      break;  
    case stopwatch_primed:
      handle_stopwatch_primed(); 
      break;

    case stopwatch_run:
      handle_stopwatch_run(); 
      break; 
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


void handle_menu(){
  setupMenu(currentMode, currentOutput);
  buttonState();

  if (okButtonState == 0 && okJustChanged){
    if (cursorInRow1){
      toggleMode();
    }
    else if (!cursorInRow1){
      toggleOutputType();
    }
  

  }
  if (backButtonState == 0 && backJustChanged){
    cursorInRow1 = !(cursorInRow1);
  }


  // start if both are pressed at the same time 

  if (okButtonState == 0 && okJustChanged && backButtonState == 0 && backJustChanged){
    if (currentMode == "timer"){
      currentState = timer_run;
    }
    else if (currentMode == "stopwatch"){
      currentState = stopwatch_paused;
    }
  }
}

void handle_timer_run(){
  digitalWrite(blueLED, HIGH);

}


void handle_stopwatch_paused(){
  digitalWrite(redLED, HIGH);
  buttonState(); 
  if (okButtonState == 0){
    Serial.print("primed stopwatch");
    currentState = stopwatch_primed;
  }
  if (backButtonState == 0){
    Serial.println("reset time");
  }

}

void handle_stopwatch_primed(){
  randomStart = random(2000, 6000);
  startRandomWait = millis(); 

  while (millis() - startRandomWait < randomStart)
  {
    buttonState();
    if (backButtonState == 0){
      currentState = stopwatch_paused;
      break;
    }
  }
  currentState = stopwatch_run;
}

void handle_stopwatch_run(){
  Serial.println("run stopwatch");
  buttonState();
  if (backButtonState == 0 || okButtonState == 0){
    Serial.println("stopped stopwatch");
    currentState = stopwatch_paused;
  return;
}






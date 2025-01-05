
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

enum State {stopwatch_primed, stopwatch_paused, stopwatch_run, timer_run, timer_paused, menu};
State currentState = menu;
State prevState = menu;


// menu variables 
  String currentOutput = "haptic";
  String currentMode = "timer";
  bool cursorInRow1 = true;  // start cursor in first row
  const int menuExitTime = 2000;
  const int pulseTime = 100; 

// pin variables 
  const int buzzerPin = 3; // using this instead of buzzer for now
  const int hapticPin = 6; // using this instead of vibration motor for now 
  const int okButton = 7; 
  const int backButton = 8; 

// stopwatch variables 
  float stopwatchSec = 0; 
  int stopwatchMin = 0;
  int randomStart = 0; 
  unsigned long startRandomWait = 0;
  unsigned long primedTime = 0; 

// timer variables 
  float timerSec = 0; 
  float timer1Sec = 10.0; 
  float timer2Sec = 5.0;
  unsigned long timerDelay = 0; 
  int timerTracker = 1; // bounces between 1 and 2

// button variables 
  const bool serialEnabled = 0; 
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
  void runStopwatch(); 
  void resetStopwatch(); 
  void handle_stopwatch_paused();
  void handle_stopwatch_run(); 
  void handle_stopwatch_primed(); 
  void pauseStopwatch(); 
  void resetStopwatch(); 

  void buttonState();
  void pauseTimer();
  void runTimer();
  void handle_timer_run();
  void handle_timer_paused();

  void setupMenu(String modeType, String outputType);
  void toggleOutputType(); 
  void toggleMode();
  void handle_menu();
  void outputPulse();








void setup() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (serialEnabled){
    Serial.begin(9600);
  }

  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();
  pinMode(okButton, INPUT_PULLUP); 
  pinMode(backButton, INPUT_PULLUP); 
  pinMode(buzzerPin, OUTPUT); 
  pinMode(hapticPin, OUTPUT); 
}



void loop() {

  switch (currentState) {

    case menu:
      handle_menu();
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

    case timer_run:
      handle_timer_run();
      break;

    case timer_paused:
      handle_timer_paused(); 
      break;
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

  if (backHoldTime > menuExitTime && backJustChanged){
    backHoldTime = 0;
    if (serialEnabled){
      Serial.print("current mode is: ");
      Serial.println(currentMode);
    }
    if (currentMode == "timer"){
      currentState = timer_paused;
      prevState = menu;
    }
    else if (currentMode == "stopwatch"){
      currentState = stopwatch_paused;
      prevState = menu;
    }
  }
}

void setupMenu(String modeType, String outputType){

  if (serialEnabled){
    Serial.print("current mode is: ");
    Serial.println(modeType);
    Serial.print("output type is: "); 
    Serial.println(outputType);

  }
  
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

void handle_stopwatch_paused(){
  pauseStopwatch();
  buttonState();
  if (backButtonState == 0 and backJustChanged){
    resetStopwatch();
  }
  if (okButtonState == 0 and okJustChanged){
    currentState = stopwatch_primed; 
    prevState = stopwatch_paused;
  }

  if (backHoldTime > menuExitTime && prevState != menu && backJustChanged){
    backHoldTime = 0;
    if (serialEnabled){
      Serial.print("current mode is: ");
      Serial.println(currentMode);
    }
    currentState = menu;
    prevState = stopwatch_paused;
  }

}

void handle_stopwatch_primed(){
  randomStart = random(2000, 6000);
  startRandomWait = millis(); 

  while (millis() - startRandomWait < randomStart)
  {
    display.clearDisplay();
    display.setTextSize(2);             
    display.setTextColor(SSD1306_WHITE);        
    display.setCursor(0,0);     
    display.print("primed");
    display.display();
    buttonState();
    if (backButtonState == 0 and backJustChanged){
      currentState = stopwatch_paused;
      prevState = stopwatch_primed;
      break;
    }
  }
  if (currentState == stopwatch_primed){
    currentState = stopwatch_run;
    prevState = stopwatch_primed;
    primedTime = (startRandomWait + randomStart);
    outputPulse();

  }

}


void handle_stopwatch_run(){
  runStopwatch();
  buttonState();
  if ((okButtonState == 0 and okJustChanged) || (backButtonState == 0 and backJustChanged)){
    currentState = stopwatch_paused;
    prevState = stopwatch_run;
  }
}

void resetStopwatch(){
  stopwatchMin = 0; 
  stopwatchSec = 0; 
  pauseStopwatch();
}

void pauseStopwatch(){
  display.clearDisplay();
  display.setTextSize(3);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0); 

  if (stopwatchMin > 0)
    {
      display.print(stopwatchMin);
      display.print(":");

      if (stopwatchSec < 10.0)
      {
        display.print("0");
        display.print(stopwatchSec); 
        display.display();
      }
      else
      {
        display.print(stopwatchSec);
        display.display();
      }
    }
    else{
      if (stopwatchSec < 10.0)
      {
        display.print("0");
        display.print(stopwatchSec); 
        display.display();
      }

      else
      {
        display.println(stopwatchSec, 2);
        display.display();

      }
    }

}

void runStopwatch(){
  display.clearDisplay();
  // at this size, assuming that we won't reach 10 minutes 
  display.setTextSize(3);             
  display.setTextColor(SSD1306_WHITE);        
  display.setCursor(0,0);             // Start at top-left corner

  stopwatchSec = (millis() - primedTime)/1000.0; 
  stopwatchMin = 0; 

  while (stopwatchSec >= 60.0)
  {
    stopwatchSec = stopwatchSec - 60.0; 
    stopwatchMin++; 
  }
  
  if (stopwatchMin > 0)
  {
    display.print(stopwatchMin);
    display.print(":");

    if (stopwatchSec < 10.0)
    {
      display.print("0");
      display.print(stopwatchSec); 
      display.display();
    }
    else
    {
      display.print(stopwatchSec);
      display.display();
    }
  }
  else{
    if (stopwatchSec < 10.0)
    {
      display.print("0");
      display.print(stopwatchSec); 
      display.display();
    }
    else
    {
      display.println(stopwatchSec, 2);
      display.display();
    }
  }
  delay(10);
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
      outputPulse();
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
      outputPulse();
      timerDelay = millis();
    }
  }
}

void handle_timer_run(){
  buttonState(); 
  runTimer();

  if ((okButtonState == 0 and okJustChanged) || (backButtonState == 0 and backJustChanged)){
    currentState = timer_paused;
    prevState = timer_run;
  }

}

void handle_timer_paused(){
  buttonState(); 
  pauseTimer(); 

  if (okButtonState == 0 and okJustChanged){
    currentState = timer_run;
    prevState = timer_paused;
    timerDelay = millis() - (timerSec*1000.0);
  }

  if (backButtonState == 0 and backJustChanged){
    timerSec = 0;
    timerDelay = millis();
  }

  if (backHoldTime > menuExitTime && prevState != menu && backJustChanged){
    backHoldTime = 0;
    if (serialEnabled){
      Serial.print("current mode is: ");
      Serial.println(currentMode);
    }
    currentState = menu;
    prevState = timer_paused;
  }

}

void outputPulse(){
  if (currentOutput == "sound"){

    digitalWrite(buzzerPin, HIGH);
    digitalWrite(hapticPin, LOW);
  }

  else if (currentOutput == "haptic"){
    digitalWrite(buzzerPin, LOW);
    digitalWrite(hapticPin, HIGH);
  }

  else if (currentOutput == "dual"){
    digitalWrite(buzzerPin, HIGH); 
    digitalWrite(hapticPin, HIGH);
  }
  else {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(hapticPin, HIGH);
  }

  delay(pulseTime);
  digitalWrite(buzzerPin, LOW);
  digitalWrite(hapticPin, LOW);

}

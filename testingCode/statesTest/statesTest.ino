/*
Date: May 25th 2024 
Description: test buttons and button debouncing
make sure one button press isn't misinterpreted as multiple bounces 
reusing some code from morse code project 

*/

enum State {stopwatch_primed, stopwatch_paused, stopwatch_run, timer_run, menu};
State currentState = menu;
int randomStart = 0; 
unsigned int startRandomWait = 0;

const int blueLED = 3; // using this instead of buzzer for now
const int redLED = 6; // using this instead of vibration motor for now 
const int okButton = 7; 
const int backButton = 8; 


char outputMode = 'h';
// 'd' is for dual, 'b' is for buzzer, 'h' is for haptic

char mode = 't';
// 't' is for timer, 's' is for stopwatch

// button variables 
long unsigned currentTime = 0; 
const int debounceDelay = 10; 
long okLastDetection;
int okButtonState = 1; // initialize these to unpressed 
int okLastButtonState = 1;
bool okPressed = 0;
unsigned long okStartPressed = 0; 
unsigned long okEndPressed = 0;
unsigned long okHoldTime = 0; 
unsigned long okIdleTime = 0;
long backLastDetection;
int backButtonState = 1;  // initialize these to unpressed 
int backLastButtonState = 1;
bool backPressed = 0;
unsigned long backStartPressed = 0; 
unsigned long backEndPressed = 0;
unsigned long backHoldTime = 0; 
unsigned long backIdleTime = 0;


// function prototypes 
  void buttonState();
  void handle_stopwatch_paused();
  void handle_stopwatch_run(); 
  void handle_stopwatch_primed(); 
  void handle_menu();
  void handle_timer_run();


void setup() {
  Serial.begin(9600); 
  pinMode(okButton, INPUT_PULLUP); 
  pinMode(backButton, INPUT_PULLUP); 
  pinMode(blueLED, OUTPUT); 
  pinMode(redLED, OUTPUT); 

}

void loop() {
  buttonState();

// enum State {stopwatch_primed, stopwatch_paused, stopwatch_run, timer_run, menu};


  switch (currentState) {
    case menu:
      handle_menu();
      break;
    case timer_run:
      handle_timer_run(); 
      break(); 
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

void handle_menu(){

}

void handle_timer_run(){

}

void handle_stopwatch_paused(){
  Serial.println("--------------------------------------------------------");
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
      if (okButtonState == 0 & okPressed == 1)
      {
        Serial.println("======just pressed ok =======");
      }
      okPressed = 0; 

    }


    // if the button is just released 
    else if (okButtonState == 1)
    {

      okEndPressed = millis();
      okHoldTime = okEndPressed - okStartPressed; 
      if (okButtonState == 1 && okPressed == 0)
      {
        Serial.print("---just released ok ----: holdtime: ");
        Serial.println(okHoldTime);
      }

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
      {
        Serial.println("======just pressed back =======");
      }
      backPressed = 0; 

    }


    // if the button is just released 
    else if (backButtonState == 1)
    {

      backEndPressed = millis();
      backHoldTime = backEndPressed - backStartPressed; 
      if (backButtonState == 1 && backPressed == 0)
      {
        Serial.print("---just released back ----: holdtime: ");
        Serial.println(backHoldTime);
      }

      backPressed = 1;
    }


  }


  okLastButtonState = okReading; 
  backLastButtonState = backReading; 



}


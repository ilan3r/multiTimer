/*
Date: May 25th 2024 
Description: test buttons and button debouncing
make sure one button press isn't misinterpreted as multiple bounces 
reusing some code from morse code project 

*/




const int okButton = 7; 
const int backButton = 8;



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

void setup() {

  pinMode(okButton, INPUT_PULLUP); 
  pinMode(backButton, INPUT_PULLUP); 

 if (serialEnabled){
  Serial.begin(9600); 
  Serial.println();
  Serial.println();
  Serial.println();
  Serial.println();
 }

}

void loop() {
  buttonState(); 
  if (okJustChanged && okButtonState == 0){
    if (serialEnabled) Serial.println("-------------------------------------------- ok pressed ");
  }
  if (backButtonState == 1 && backJustChanged){
    if (serialEnabled) Serial.println("----------------------------------------------- back pressed ");
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

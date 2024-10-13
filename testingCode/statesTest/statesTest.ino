 /* code to test the states of the timer and switching between them 

 */



enum State {menu, timer_run, stopwatch_run, stopwatch_pause};
State currentState;

const int okButton = 7; 
const int backButton = 8; 

long unsigned currentTime = 0; 

const int debounceDelay = 10; 
long okLastDetection;
int okButtonState = 0; 
int okLastButtonState = 0;
bool okPressed = 0;
unsigned long okStartPressed = 0; 
unsigned long okEndPressed = 0;
unsigned long okHoldTime = 0; 
unsigned long okIdleTime = 0;

long backLastDetection;
int backButtonState = 0; 
int backLastButtonState = 0;
bool backPressed = 0;
unsigned long backStartPressed = 0; 
unsigned long backEndPressed = 0;
unsigned long backHoldTime = 0; 
unsigned long backIdleTime = 0;


void setup() {
  Serial.begin(9600); 
  pinMode(okButton, INPUT_PULLUP); 
  pinMode(backButton, INPUT_PULLUP); 
}



void loop() {
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

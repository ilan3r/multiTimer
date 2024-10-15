/*
Date: May 25th 2024 
Description: test buttons and button debouncing
make sure one button press isn't misinterpreted as multiple bounces 
reusing some code from morse code project 

*/

const int blueLED = 3; // using this instead of buzzer for now
const int redLED = 6; // using this instead of vibration motor for now 
     
const int okButton = 7; 
const int backButton = 8;


long unsigned currentTime = 0; 


// the time allowed for button debouncing 
const int debounceDelay = 10; 

long lastDetection; 

// the current and previous state of the button
int okButtonState = 0;     
int okLastButtonState = 0; 
bool okPressed = 0; 

// the moment the button was pressed and released
unsigned long startPressed = 0;    
unsigned long endPressed = 0;   

int randomStart = 0;
int startWaitTime = 0; 

// the time the button has held for and been released for 
unsigned long holdTime = 0;  
unsigned long idleTime = 0; 
bool primed = false; 
bool started = false; 

void setup() {
  Serial.begin(9600); 
  pinMode(okButton, INPUT_PULLUP); 
  pinMode(backButton, INPUT_PULLUP); 
  pinMode(blueLED, OUTPUT); 
  pinMode(redLED, OUTPUT); 

}

void loop() {
  buttonState();


  if (okPressed == 0 && primed == false && started == false){
    primed = true; 
    startWaitTime = millis(); 
    randomStart = random(2000, 6000);

  }

  if (okPressed == 0 && primed == false && started == true){
    started == false;
    Serial.println("--------------------------------------------- reset ");
  }



  else if (primed==true && (millis()-startWaitTime <= randomStart) && started == false){
    digitalWrite(redLED, HIGH);
    Serial.println("--------------------------------------------- PRIMED");
  }

  else if (primed == true && (millis()-startWaitTime >= randomStart) && started == false){
    digitalWrite(redLED, LOW);
    digitalWrite(blueLED, HIGH);
    started = true;
    Serial.println("------------------------------------------------ Started"); 
  }

  





}


// note that okPressed is what tracks if the button is pressed
// when okPressed == 1, it is unpressed
void buttonState(){
  int okReading = digitalRead(okButton); 

  if (okButtonState != okLastButtonState){
    lastDetection = millis();
  }

  // only accept the change in button state after debounce time
  if ( (millis() - lastDetection) > debounceDelay ){
    okButtonState = okReading; 
  }
  // if the button is pressed or released
  if (okButtonState != okLastButtonState)
  {

    // if the button is just pressed 
    if (okButtonState == 0)
    {
      startPressed = millis(); 
      if (okButtonState == 0 & okPressed == 1)
      {
        Serial.println("======just pressed=======");
      }
      okPressed = 0; 

    }
    // if the button is just released 
    else if (okButtonState == 1)
    {

      endPressed = millis();
      holdTime = endPressed - startPressed; 
      if (okButtonState == 1 && okPressed == 0)
      {
        Serial.print("---just released----: holdtime: ");
        Serial.println(holdTime);
      }

      okPressed = 1;
    }
  }
  okLastButtonState = okReading; 
}
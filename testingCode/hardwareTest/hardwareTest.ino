/*
Date: May 17th 2024 
Description: test all inputs and outputs on the breadboard version: 
- 2 buttons
- 2 LEDs 

*/

const int blueLED = 3; // using this instead of buzzer for now
const int redLED = 6; // using this instead of vibration motor for now 

const int okButton = 7; 
const int backButton = 8; 


void setup() {
  Serial.begin(9600);
  pinMode(blueLED, OUTPUT); 
  pinMode(redLED, OUTPUT); 
  pinMode(okButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP); 

}

void loop() {

  if (digitalRead(okButton) == LOW){
    Serial.println("ok===============================================");
  }
  if (digitalRead(backButton) == LOW){
    Serial.println("back----");
  }

  digitalWrite(blueLED, HIGH); 
  // digitalWrite(redLED, LOW); 
  delay(500); 
  digitalWrite(blueLED, LOW); 
  // digitalWrite(redLED, HIGH); 
  delay(500);

}

/**
 * Smart Petfood Dispenser 
 * @fanman, @sanjitdaddy, @roland
*/
//#include <LiquidCrystal.h>
const int LED = 13; // Led Light
const int RELAY = 8; // Dispenser Relay
const int BUTTON = 3; // Button

int buttonState = 0;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUTTON, INPUT);
  
  digitalWrite(RELAY, HIGH); // Turn off the relay at the start
}

void loop() {
  buttonState = digitalRead(BUTTON);
  
  if (buttonState == HIGH) {
    digitalWrite(LED, HIGH);
    digitalWrite(RELAY, LOW);
    delay(500);
    digitalWrite(LED, LOW);
    digitalWrite(RELAY, HIGH);
  }

}

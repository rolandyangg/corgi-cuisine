/**
 * Smart Petfood Dispenser 
 * @fanman, @sanjit, @roland
*/
const int GREEN_LED = 13; // Green Light - Signals whenever it is valid to get food
const int YELLOW_LED = 6; // Yellow Light - Signals when it is in its cooldown period
const int RED_LED = 2; // Red Light - Signals when it is invalid to get food
const int RELAY = 8; // Dispenser Relay
const int BUTTON = 3; // Button
const int BUZZER = 5; // Music player
const bool IS_AUTOMATIC = false;

// THIS IS THE COOLDOWN
// THIS IS THE COOLDOWN
// THIS IS THE COOLDOWN
const unsigned long COOLDOWN = 5000; // Cooldown time to reset to in MILLISECONDS 
// THIS IS THE COOLDOWN
// THIS IS THE COOLDOWN
// THIS IS THE COOLDOWN

int buttonState = 0; // Whether the button is pressed or not
unsigned long endTime = 0; // Time at the end of the cooldown
bool isInCooldown = false;

void setup() {
  // Setup the input and outputs correctly
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(BUTTON, INPUT);

  digitalWrite(RELAY, HIGH); // Turn off the relay at the start
  digitalWrite(GREEN_LED, HIGH); // Make it automatically valid

  Serial.begin(9600); // Start up console
}

void loop() {
  buttonState = digitalRead(BUTTON);
  
  if (buttonState == HIGH) {
    if (digitalRead(GREEN_LED) == HIGH) { // Valid to press, make sure the Corgi does not become overweight by eating too much!!!
      dispense();
    } 
    else { // Not valid to press, must wait for the cooldown time
      error();
    }
  }
  else if (isInCooldown && millis() > endTime) { // Checks the cooldown
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      isInCooldown = false; // Only have to care about whether in cooldown or not at this point, no need to change endTime
      if (IS_AUTOMATIC) {
        for (int i = 0; i < 5; i++) {// Blink for 5 seconds before redispensing
          digitalWrite(GREEN_LED, LOW);
          tone(BUZZER, 500);
          fakeDelay(500);
          noTone(BUZZER);
          digitalWrite(GREEN_LED, HIGH);
          fakeDelay(500);
        }
        dispense();
      }
  }

}

void fakeDelay(int time) {
  unsigned long tempEndTime = millis() + time;
  while (tempEndTime - millis() > (unsigned long) 0) {
    if (millis() > tempEndTime) // safeguard incase it spazzes out
      break;
  }
}

void dispense() {
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(RELAY, LOW);
  digitalWrite(GREEN_LED, LOW);
  fakeDelay(1500);
  digitalWrite(RELAY, HIGH);
  // Setup timer
  endTime = millis() + COOLDOWN; // Does not account for the 50 day overflow
  isInCooldown = true;
}

void error() {
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, HIGH);
  tone(BUZZER, 1000);
  fakeDelay(750);
  noTone(BUZZER);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(RED_LED, LOW);
}

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
const int COOLDOWN = 500;

int buttonState = 0; // Whether the button is pressed or not
int cooldownLeft = 0;

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
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(RELAY, LOW);
      digitalWrite(GREEN_LED, LOW);
      delay(1250);
      digitalWrite(RELAY, HIGH);
    } else { // Not valid to press, must wait for the cooldown time
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(RED_LED, HIGH);
      tone(BUZZER, 1000);
      delay(750);
      noTone(BUZZER);
      digitalWrite(YELLOW_LED, HIGH);
      digitalWrite(RED_LED, LOW);
    }
  }
  
}

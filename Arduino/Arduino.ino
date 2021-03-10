/**
 * Smart Petfood Dispenser 
 * @fanman, @sanjit, @roland
*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

// Songs
// Mario
int melody1[] = {
  NOTE_E3, NOTE_E3, NOTE_E3, NOTE_C3, NOTE_E3, NOTE_G4, NOTE_G3
};
int noteDurations1[] = {
  6, 4, 4, 6, 4, 2, 4
};
// Skunk In The Barnyard
int melody2[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3
};
int noteDurations2[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};

const int GREEN_LED = 13; // Green Light - Signals whenever it is valid to get food
const int YELLOW_LED = 6; // Yellow Light - Signals when it is in its cooldown period
const int RED_LED = 2; // Red Light - Signals when it is invalid to get food
const int RELAY = 8; // Dispenser Relay
const int BUTTON = 3; // Button
const int BUZZER = 5; // Music player

const bool IS_AUTOMATIC = false; // Whether the whole entire thing stays in a loop or not
const unsigned long COOLDOWN = 10000; // Cooldown time to reset to in MILLISECONDS 

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
    if (digitalRead(GREEN_LED) == HIGH) // Valid to press, make sure the Corgi does not become overweight by eating too much!!!
      dispense();
    else // Not valid to press, must wait for the cooldown time
      error();
  }
  else if (isInCooldown && millis() > endTime) { // Checks the cooldown
      playSong(melody1, noteDurations1);
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      isInCooldown = false; // Only have to care about whether in cooldown or not at this point, no need to change endTime
      if (IS_AUTOMATIC) { // Keep it in a never ending loop if choose to do automatic
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
  playSong(melody2, noteDurations2);
  digitalWrite(YELLOW_LED, HIGH);
  digitalWrite(RELAY, LOW);
  digitalWrite(GREEN_LED, LOW);
  for (int i = 0; i < 4; i++) {
  fakeDelay(300);
  digitalWrite(RELAY, HIGH);
  fakeDelay(300);
  digitalWrite(RELAY, LOW);
  }
  fakeDelay(300);
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

void playSong(int melody[], int noteDurations[]) {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(5, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(5);
  }
}

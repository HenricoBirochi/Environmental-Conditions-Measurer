#include "pitches.h"

#define SPEAKER_PIN 3


int melodyPirate[] = {
  NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_C4, NOTE_D4,
  NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_C4, NOTE_D4,
  NOTE_A3, NOTE_C4, NOTE_D4, NOTE_D4, NOTE_D4, NOTE_F4, NOTE_G4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_D4,
  NOTE_D4, NOTE_E4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_D4, NOTE_D4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_F4, NOTE_D4, NOTE_E4
};

float noteDurationsPirate[] = {
  2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2, 0.67, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 
  0.5, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 2, 2, 2, 0.67, 2, 2, 1, 1, 1, 2, 0.67, 2, 2, 1, 1, 2, 2, 0.5
};

void setup() {
  playMelodyPirate();

}

void loop() {
  // put your main code here, to run repeatedly:

}

void playMelodyPirate() {
    for (int thisNote = 0; thisNote < 61; thisNote++) {

    float noteDuration = 150 / noteDurationsPirate[thisNote];
    tone(SPEAKER_PIN, melodyPirate[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.40;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(SPEAKER_PIN);
  }
}

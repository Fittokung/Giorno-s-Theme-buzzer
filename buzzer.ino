#include <Arduino.h>

#define BUZZER_PIN 32
#define button 35

//ใช้ Octave ที่ 5 (A,B ใช้ Octave ที่ 4)
//B B BA B D B F#A 
//B B BA B F E DA
const int GiornoNotes[] = {494, 494, 494, 440, 494, 587.5, 494, 370, 440, 494, 494, 494, 440, 494, 698.5, 659.5, 587.5, 440};
const int GiornoDurations[] = {225, 225, 125, 225, 225, 225, 225, 125, 225, 225, 225, 125, 225, 225, 225, 225, 125, 225};

volatile int noteIndex = 0;
volatile bool playingNote = false;
int i = 0 ;

hw_timer_t *timer = NULL;

void IRAM_ATTR onTimer() {
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();

  if (!playingNote) {
    int frequency = GiornoNotes[noteIndex];
    int duration = GiornoDurations[noteIndex];

    if (frequency == 0) {
      delay(duration);  // Pause between notes
    } else {
      tone(BUZZER_PIN, frequency);
      playingNote = true;
      previousMillis = currentMillis;
    }

    noteIndex = (noteIndex + 1) % (sizeof(GiornoNotes) / sizeof(GiornoNotes[0]));  // Move to the next note
  }

  if (playingNote && (currentMillis - previousMillis >= GiornoDurations[noteIndex])) {
    noTone(BUZZER_PIN);
    playingNote = false;
  }
}

void IRAM_ATTR IO_INT_ISR()
{
  // Speed
  i=((i+1)%5);
  switch (i) {
  case 0:  timerAlarmWrite(timer, 10000, true);
        break;
  case 1:  timerAlarmWrite(timer, 1000, true); 
        break;
  case 2:  timerAlarmWrite(timer, 100, true);
        break;
  case 3:  timerAlarmWrite(timer, 100000, true);
        break;  
  case 4:  timerAlarmWrite(timer, 500000, true);
        break;
  }
  timerAlarmEnable(timer);
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(button, IO_INT_ISR,FALLING);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 10000, true);
  timerAlarmEnable(timer);
}

void loop() {

}
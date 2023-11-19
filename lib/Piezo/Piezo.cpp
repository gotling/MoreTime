#include <Piezo.h>

#include <arduino-timer.h>

auto piezoTimer = timer_create_default();

unsigned long nextTone = 0;
bool piezoActive = false;

double toneFrequency[] = {1319, 0, 1319, 0, 1568, 0, 1047, 0};
double toneLength[] =    {150, 20, 150, 20, 150, 20, 200, 1000};
unsigned int toneCount = sizeof(toneFrequency)/sizeof(double);
unsigned int tonePosition = 0;
unsigned int piezoRepeat = 3;

void piezoSetup()
{
  pinMode(PIEZO_PIN, OUTPUT);
  digitalWrite(PIEZO_PIN, 0);
}

void piezoPlay()
{
  Serial.println("Piezo play");
  if (piezoActive)
    return;
  
  tonePosition = 0;
  piezoRepeat = 3;
  nextTone = millis() + toneLength[tonePosition];
  piezoActive = true;
}

void piezoStop()
{
  piezoActive = false;
  noTone(PIEZO_PIN);
}

void piezoLoop()
{
  if (!piezoActive)
    return;
  
  if (tonePosition >= toneCount) {
    piezoRepeat--;
    if (piezoRepeat <= 0) {
      piezoStop();
      return;
    } else {
      tonePosition = 0;
    }
  }
  
  if (millis() > nextTone) {
    if (toneFrequency[tonePosition] == 0) {
      noTone(PIEZO_PIN);
    } else {
      tone(PIEZO_PIN, toneFrequency[tonePosition], toneLength[tonePosition]);
    }

    nextTone = millis() + toneLength[tonePosition];
    tonePosition++;
  }
}
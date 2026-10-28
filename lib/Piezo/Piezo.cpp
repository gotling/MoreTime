#include <Piezo.h>

#include <arduino-timer.h>

auto piezoTimer = timer_create_default();

unsigned long nextTone = 0;
bool piezoActive = false;

double toneFrequency[] = {2000, 0, 2000, 0, 3000, 0, 2000, 0};
double toneLength[] =    {100, 20, 100, 20, 100, 20, 200, 500};
unsigned int toneCount = sizeof(toneFrequency)/sizeof(double);
unsigned int tonePosition = 0;
unsigned int piezoRepeat = 3;

void piezoSetup()
{
  piezoIdle();
}

// Leave the pin high-impedance when silent. Driven low, the piezo plays
// supply noise from the display and WiFi as an audible hum.
void piezoIdle()
{
  pinMode(PIEZO_PIN, INPUT);
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
  piezoIdle();
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
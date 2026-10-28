#include <Piezo.h>

#include <Arduino.h>
#include <EEPROM.h>
#include <core_esp8266_waveform.h>

// Piezo buzzers are loudest around 2-4 kHz
const Note classicNotes[] = {{2000, 100}, {0, 20}, {2000, 100}, {0, 20}, {3000, 100}, {0, 20}, {2000, 200}, {0, 500}};
const Note chimeNotes[] = {{2093, 250}, {2637, 250}, {3136, 250}, {4186, 600}, {0, 900}};
const Note riseNotes[] = {{2093, 150}, {2637, 150}, {3136, 150}, {4186, 400}, {0, 600}};
const Note fallNotes[] = {{4186, 200}, {3136, 200}, {2637, 200}, {2093, 400}, {0, 700}};
const Note sparkNotes[] = {{2093, 110}, {2637, 110}, {3136, 110}, {4186, 110}, {3136, 110}, {2637, 110}, {2093, 300}, {0, 700}};
const Note twinNotes[] = {{3136, 180}, {4186, 350}, {0, 150}, {3136, 180}, {4186, 350}, {0, 800}};
const Note beepNotes[] = {{2730, 120}, {0, 100}, {2730, 120}, {0, 100}, {2730, 120}, {0, 1000}};
const Note tuneNotes[] = {{2093, 150}, {2637, 150}, {3136, 150}, {2637, 150}, {3136, 150}, {4186, 500}, {0, 900}};

#define NOTES(n) n, sizeof(n) / sizeof(Note)

const Sound sounds[] = {
  {"Classic", NOTES(classicNotes), false, 3},
  {"Chime", NOTES(chimeNotes), true, 3},
  {"Rise", NOTES(riseNotes), true, 4},
  {"Fall", NOTES(fallNotes), true, 3},
  {"Spark", NOTES(sparkNotes), true, 3},
  {"Twin", NOTES(twinNotes), true, 3},
  {"Beep", NOTES(beepNotes), true, 4},
  {"Tune", NOTES(tuneNotes), true, 2},
};
const uint8_t soundCount = sizeof(sounds) / sizeof(Sound);

// Duty cycle in per mille. 50% is loudest, lower is quieter.
#define DUTY_MAX 500
#define DUTY_MIN 40
// Part of each note played at full volume before the fade starts
#define DECAY_SUSTAIN 0.3f
#define DECAY_STEP_MS 10

#define EEPROM_SIZE 4
#define EEPROM_SOUND_ADDRESS 0

// Spark, used until a sound is picked in the menu
#define DEFAULT_SOUND 4

uint8_t alarmSound = DEFAULT_SOUND;

const Sound *playing = nullptr;
uint8_t notePosition = 0;
uint8_t repeatLeft = 0;
unsigned long noteStart = 0;
unsigned long lastDecayStep = 0;

void piezoSetup()
{
  piezoIdle();

  EEPROM.begin(EEPROM_SIZE);
  uint8_t stored = EEPROM.read(EEPROM_SOUND_ADDRESS);
  // Unwritten EEPROM reads 0xFF
  alarmSound = stored < soundCount ? stored : DEFAULT_SOUND;
}

// Leave the pin high-impedance when silent. Driven low, the piezo plays
// supply noise from the display and WiFi as an audible hum.
void piezoIdle()
{
  pinMode(PIEZO_PIN, INPUT);
}

void piezoTone(uint16_t frequency, uint16_t duty)
{
  uint32_t period = 1000000UL / frequency;
  uint32_t high = period * duty / 1000;
  if (high == 0)
    high = 1;

  // Changes a running waveform on the fly
  startWaveform(PIEZO_PIN, high, period - high);
}

void piezoStartNote()
{
  const Note &note = playing->notes[notePosition];
  noteStart = millis();
  lastDecayStep = noteStart;

  if (note.frequency == 0) {
    stopWaveform(PIEZO_PIN);
    piezoIdle();
  } else {
    pinMode(PIEZO_PIN, OUTPUT);
    piezoTone(note.frequency, DUTY_MAX);
  }
}

void piezoStart(const Sound *sound, uint8_t repeat)
{
  playing = sound;
  notePosition = 0;
  repeatLeft = repeat;
  piezoStartNote();
}

void piezoPlay()
{
  if (playing)
    return;

  Serial.print("Piezo play: ");
  Serial.println(sounds[alarmSound].name);
  piezoStart(&sounds[alarmSound], sounds[alarmSound].repeat);
}

void piezoPreview(uint8_t index)
{
  piezoStart(&sounds[index], 1);
}

void piezoStop()
{
  playing = nullptr;
  stopWaveform(PIEZO_PIN);
  piezoIdle();
}

uint8_t piezoSound()
{
  return alarmSound;
}

void piezoSetSound(uint8_t index)
{
  alarmSound = index;
  EEPROM.write(EEPROM_SOUND_ADDRESS, index);
  EEPROM.commit();
}

void piezoLoop()
{
  if (!playing)
    return;

  const Note &note = playing->notes[notePosition];
  unsigned long elapsed = millis() - noteStart;

  if (elapsed >= note.length) {
    notePosition++;
    if (notePosition >= playing->count) {
      notePosition = 0;
      repeatLeft--;
      if (repeatLeft == 0) {
        piezoStop();
        return;
      }
    }
    piezoStartNote();
    return;
  }

  if (playing->decay && note.frequency != 0 && millis() - lastDecayStep >= DECAY_STEP_MS) {
    lastDecayStep = millis();
    float progress = (float)elapsed / note.length;
    if (progress > DECAY_SUSTAIN) {
      float left = 1.0f - (progress - DECAY_SUSTAIN) / (1.0f - DECAY_SUSTAIN);
      piezoTone(note.frequency, DUTY_MIN + (DUTY_MAX - DUTY_MIN) * left);
    }
  }
}

#include <Sound.h>

#include <Arduino.h>

#include <Display.h>
#include <State.h>
#include <Piezo.h>
#include <MyTime.h>

uint8_t soundIndex = 0;

void soundOpen() {
  state.mode = SoundSelect;
  soundIndex = piezoSound();
  displayFontDefault();
  soundDisplay();
}

void soundClose() {
  piezoStop();
}

void soundMinus() {
  soundIndex = (soundIndex + soundCount - 1) % soundCount;
  soundDisplay();
  piezoPreview(soundIndex);
}

void soundPlus() {
  soundIndex = (soundIndex + 1) % soundCount;
  soundDisplay();
  piezoPreview(soundIndex);
}

void soundAction() {
  piezoSetSound(soundIndex);
  Serial.print("Sound: saved ");
  Serial.println(sounds[soundIndex].name);
  soundClose();
  timeOpen();
}

// A trailing * marks the saved alarm sound
void soundDisplay() {
  bool selected = soundIndex == piezoSound();
  snprintf(message, BUF_SIZE, "%s%s", sounds[soundIndex].name, selected ? "*" : "");
  displayPrint(message);

  Serial.print("Sound: ");
  Serial.print(sounds[soundIndex].name);
  Serial.println(selected ? " (selected)" : "");
}

#include <Sound.h>

#include <Arduino.h>

#include <Display.h>
#include <Piezo.h>
#include <MyTime.h>

SoundMode soundMode;

void SoundMode::open() {
  index = piezoSound();
  displayFontDefault();
  display();
}

void SoundMode::close() {
  piezoStop();
}

void SoundMode::minus() {
  index = (index + soundCount - 1) % soundCount;
  preview();
}

void SoundMode::plus() {
  index = (index + 1) % soundCount;
  preview();
}

void SoundMode::action() {
  piezoSetSound(index);
  Serial.print("Sound: saved ");
  Serial.println(sounds[index].name);
  modeSwitch(&timeMode);
}

void SoundMode::preview() {
  display();
  piezoPreview(index);
}

// A trailing * marks the saved alarm sound
void SoundMode::display() {
  bool selected = index == piezoSound();
  displayPrintf("%s%s", sounds[index].name, selected ? "*" : "");

  Serial.print("Sound: ");
  Serial.print(sounds[index].name);
  Serial.println(selected ? " (selected)" : "");
}

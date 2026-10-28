#ifndef _MODE_H_
#define _MODE_H_

// A screen on the device, like the clock or the timer. One mode is active at
// a time and receives the button presses. A new mode inherits from Mode,
// overrides the methods it needs, and is added to the menu in Menu.cpp.
class Mode {
public:
  // Label shown in the menu and serial log
  virtual const char *name() = 0;

  virtual void open() {}
  virtual void close() {}

  virtual void minus() {}
  virtual void plus() {}
  virtual void action() {}
  // Holding the action button returns to the clock unless overridden
  virtual void hold();

  // Called every updateInterval ms while the mode is active
  virtual void update() {}

protected:
  // 0 means update on every loop
  unsigned long updateInterval = 0;

  // Wait a full interval before the next update, e.g. after a button press
  void restartUpdates();

private:
  unsigned long lastUpdate = 0;

  friend void modeSwitch(Mode *mode);
  friend void modeLoop();
};

// Close the active mode and open a new one
extern void modeSwitch(Mode *mode);
extern Mode *modeCurrent();
extern void modeLoop();

#endif

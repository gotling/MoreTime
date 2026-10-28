#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#include <Arduino.h>

#include <Mode.h>

#define MESSAGE_COUNT 4

// Shows messages received over MQTT. Minus/plus browse, action returns to the clock.
class MessageMode : public Mode {
public:
  const char *name() override { return "Message"; }
  void open() override;
  void minus() override;
  void plus() override;
  void action() override;

  // Set a message from JSON: {"position": 0-3, "value": "text"}
  void add(String &json);

private:
  String messages[MESSAGE_COUNT] = {"0", "1", "2", "3"};
  unsigned short displayIndex = 0;

  void display();
};

extern MessageMode messageMode;

#endif

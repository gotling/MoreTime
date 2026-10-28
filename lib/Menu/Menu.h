#ifndef _MENU_H_
#define _MENU_H_

#include <Mode.h>

// Lists the modes in menuItems. Minus/plus browse, action opens.
// Returns to the clock after a while without button presses.
class MenuMode : public Mode {
public:
  MenuMode();
  const char *name() override { return "Menu"; }
  void open() override;
  void minus() override;
  void plus() override;
  void action() override;
  // The hold that follows the press opening the menu should not close it
  void hold() override {}
  void update() override;

private:
  int index = 0;

  void display();
};

extern MenuMode menuMode;

#endif

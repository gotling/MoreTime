# MoreTime

A device with a screen and buttons that can show time and more.

This is a replacement for JustATimer that was made in Arduino IDE and became to complicated to maintain.

Built using PlatformIO.

# Code structure

Each screen, like the clock, timer or menu, is a mode in `lib/`. A mode is a class inheriting from `Mode` (`lib/Mode/Mode.h`) that overrides what it needs: `open`, `close`, the buttons `minus`, `plus`, `action`, `hold`, and `update`, which is called every `updateInterval` ms while the mode is shown. Switch screens with `modeSwitch(&otherMode)`, which always closes the current mode first.

To add a mode:

1. Create `lib/MyMode/MyMode.h` and `.cpp` with a class inheriting from `Mode` and a global instance, e.g. `extern MyMode myMode;`.
2. Add `&myMode` to `menuItems` in `lib/Menu/Menu.cpp`.

# Hardware

* ESP8266
* 3 push buttons
* 8x8x4 MAX7219 control dot matrix
* Piezo speaker

# Case

Designed in Fusion 360.

# Libraries
## MD_MAX72xx

https://github.com/MajicDesigns/MD_MAX72XX

## MD_Parola

https://github.com/MajicDesigns/MD_Parola

Configure font: https://pjrp.github.io/MDParolaFontEditor

## WiFiManager

https://github.com/tzapu/WiFiManager

On first boot, or if the saved network can't be reached, the device shows "Setup" and opens an access point named `MoreTime-<chip id>`. Connect to it and pick a WiFi network in the portal. Time zone is set in `lib/NTP/NTP.h`.
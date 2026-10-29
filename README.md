# MoreTime

A device with a screen and buttons that can show time and more.

This is a replacement for JustATimer that was made in Arduino IDE and became to complicated to maintain.

Built using PlatformIO.

# Code structure

Each screen, like the clock, timer or menu, is a mode in `lib/`. A mode is a class inheriting from `Mode` (`lib/Mode/Mode.h`) that overrides what it needs: `open`, `close`, the buttons `minus`, `plus`, `action`, `hold`, and `update`, which is called every `updateInterval` ms while the mode is shown. Switch screens with `modeSwitch(&otherMode)`, which always closes the current mode first.

To add a mode:

1. Create `lib/MyMode/MyMode.h` and `.cpp` with a class inheriting from `Mode` and a global instance, e.g. `extern MyMode myMode;`.
2. Add `&myMode` to `menuItems` in `lib/Menu/Menu.cpp`.

# Phone control

Each device serves a web page for starting, pausing and stopping the timer and stopwatch. After connecting to WiFi the device scrolls its IP address once, and the IP menu item shows it again. Open it in the phone's browser. The name `moretime-<chip id>.local` works on computers and iPhones but often not on Android, where the IP address is the reliable choice. The serial log prints both. Add the page to the home screen to use it like an app.

The page uses a small API that also works from automation apps:

| Request | Does |
|---|---|
| `GET /api/state` | Current state as JSON |
| `POST /api/timer/start?seconds=300` | Start a timer, replacing any running one |
| `POST /api/timer/pause`, `/resume`, `/stop` | Control the running timer, stop returns to the clock |
| `POST /api/stopwatch/start` | Start the stopwatch from zero |
| `POST /api/stopwatch/pause`, `/resume`, `/stop` | Control the running stopwatch |

For example `curl -X POST "http://moretime-a1b2c3.local/api/timer/start?seconds=300"`.

# Several devices

Devices on the same network show each other's timer, stopwatch and chess clock. While one is running or paused, the device broadcasts its state over UDP port 4210 every second. Another device showing the clock switches to mirror it, sounds the alarm when it ends and returns to the clock when it stops or nothing is heard for 5 seconds. Pressing a button on the mirroring device returns it to the clock until the next session. All devices run the same firmware, a device without buttons simply only mirrors.

To watch the messages from a computer on the same network:

    nc -ul 4210

# Tests

Unit tests run on the computer, not the device:

    pio test -e native

Each `test/test_*` folder is a suite. The modes are compiled as they are, while `test/fakes` replaces Arduino, the display and the piezo, so a test can move time forward with `advance(ms)` and check what the display shows. Only libraries listed under `env:native` in `platformio.ini` are built, so add a mode there to test it.

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
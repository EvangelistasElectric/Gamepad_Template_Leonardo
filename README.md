# Gamepad_Template_Leonardo
# Leonardo HID Joystick & Pedal Controller

This project turns an **Arduino Leonardo** (ATmega32U4) into a **custom USB HID game controller** with:

- Two analog joysticks (4 axes total)
- Two analog pedals (1 axis each)
- Multiple digital buttons (configurable via pins)
- HID-compliant gamepad output (works in Windows, Linux, Mac, and most games)

Designed for **simulator and custom control setups** where you need full flexibility over button mapping and axis configuration.

---

## Features

- **6 analog axes** total:
  - Joystick 1: X, Y
  - Joystick 2: Rx, Ry
  - Pedal 1: Z
  - Pedal 2: Rz
- **12+ buttons** mapped to digital pins
- **Deadzone filtering** for analog inputs
- **Analog oversampling** for smoother readings
- Invert axis individually via `INVERT[]` array
- **Cross-platform HID support** using [Arduino Joystick Library (Matthew Heironimus)](https://github.com/MHeironimus/ArduinoJoystickLibrary)

---

## Hardware Requirements

- **Arduino Leonardo** (or any ATmega32U4 board with native USB)
- Two 2-axis joysticks (potentiometer type)
- Two single-axis pedals (potentiometer type)
- Pushbuttons (momentary, normally open)
- Hookup wire, breadboard or soldered connections

---

## Pin Mapping

| Function           | Pin |
|--------------------|-----|
| Joystick 1 X       | A0  |
| Joystick 1 Y       | A1  |
| Joystick 2 X (Rx)  | A2  |
| Joystick 2 Y (Ry)  | A3  |
| Pedal 1 (Z)        | A4  |
| Pedal 2 (Rz)       | A5  |
| Button 1           | D2  |
| Button 2           | D3  |
| Button 3           | D4  |
| Button 4           | D5  |
| Button 5           | D6  |
| Button 6           | D7  |
| Button 7           | D8  |
| Button 8           | D9  |
| Button 9           | D10 |
| Button 10          | D11 |
| Button 11          | D12 |
| Button 12          | D13 |

> **Note:** Any unused analog pins should be tied to GND or configured as `INPUT_PULLUP` to avoid floating values.

---

## Wiring Overview

- **Analog Joysticks:**
  - X/Y potentiometers → A0, A1 (Joystick 1)
  - X/Y potentiometers → A2, A3 (Joystick 2)
  - Each pot: middle pin = signal, side pins = GND and +5V

- **Pedals:**
  - Potentiometer → A4 (Pedal 1), A5 (Pedal 2)
  - Middle pin = signal, side pins = GND and +5V

- **Buttons:**
  - One side to pin (D2–D13)
  - Other side to GND
  - Using `INPUT_PULLUP`, so pressed = LOW

---

## Software Setup

### 1. Install Arduino IDE & Joystick Library
- Install [Arduino IDE](https://www.arduino.cc/en/software)
- Open Library Manager (**Sketch → Include Library → Manage Libraries…**)
- Search for **"Joystick"** by *Matthew Heironimus* and install

### 2. Select Board & Port
- **Tools → Board → Arduino Leonardo**
- **Tools → Port → (your Leonardo)**

### 3. Upload the Code
- Open `leonardo_hid.ino` (main sketch)
- Verify & Upload
- Unplug/replug USB after first upload so HID enumerates correctly

---

## Linux Testing

### CLI (`jstest`)
```bash
sudo apt install joystick
sudo modprobe joydev
jstest /dev/input/js0
GUI (jstest-gtk)
bash
Copy
Edit
sudo apt install jstest-gtk
jstest-gtk
Web Tester
Open https://gamepad-tester.com in Chrome/Firefox and check live input values.

Windows Testing
Press Win + R, type:

Copy
Edit
joy.cpl
Select your Leonardo controller

Move joysticks / press buttons to verify mapping

Customization
Invert Axis:
Edit the INVERT[] array:

cpp
Copy
Edit
bool INVERT[6] = {false, false, false, false, false, false};
Change Deadzone:

cpp
Copy
Edit
const uint16_t DEADZONE = 16;
Change Axis Order:
Modify AXIS_PINS[] mapping in the code.

Add/Remove Buttons:
Adjust the buttonPins[] array and NUM_BUTTONS definition.

Known Issues
Unused analog pins will produce random movement unless tied down or given pull-ups.

Linux mapping order may differ from Windows — test and adjust AXIS_PINS[] / buttonPins[] as needed.

HID mapping changes require unplug/replug after flashing.

🛡️ License
This project is licensed under the Evangelista's Electric Non-Commercial Open Source License (EENOSL). Please see LICENSE.txt for full terms.


Credits
Arduino Joystick Library by Matthew Heironimus

Project wiring and HID mapping by [Your Name]

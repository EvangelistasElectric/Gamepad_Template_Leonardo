# 🎮 Gamepad_Template_Leonardo  
## Leonardo HID Joystick & Pedal Controller

This project transforms an **Arduino Leonardo** (ATmega32U4) into a **custom USB HID game controller** with:

- 🎯 Two analog joysticks (4 axes total)  
- 🦶 Two analog pedals (1 axis each)  
- 🔘 Multiple digital buttons (configurable via pins)  
- 🌐 HID-compliant gamepad output (Windows, Linux, Mac, most games)  

Designed for **simulator and custom control setups** where you need full flexibility over button mapping and axis configuration.

---

## 🛡️ License
This project is licensed under the **Evangelista's Electric Non-Commercial Open Source License (EENOSL)**.  
Please see **LICENSE.txt** for full terms.

---

## 📜 Credits
- **Arduino Joystick Library** by [Matthew Heironimus](https://github.com/MHeironimus/ArduinoJoystickLibrary)  
- **Project wiring and HID mapping** by *Matthew Evangelista*  

---

## ✨ Features
- **6 Analog Axes**:
  - 🎮 Joystick 1: X, Y
  - 🎮 Joystick 2: Rx, Ry
  - 🦶 Pedal 1: Z
  - 🦶 Pedal 2: Rz
- **12+ Digital Buttons**
- 🎯 Deadzone filtering for analog inputs
- 🔄 Analog oversampling for smoother readings
- ↔ Axis inversion via `INVERT[]` array
- 🌐 Cross-platform HID support

---

## 🔧 Hardware Requirements
- **Arduino Leonardo** (or ATmega32U4 board with native USB)
- 2× 2-axis joysticks (potentiometer type)
- 2× single-axis pedals (potentiometer type)
- Pushbuttons (momentary, normally open)
- Hookup wire, breadboard or soldering

---

## 🖇️ Pin Mapping

| Function           | Pin  |
|--------------------|------|
| 🎮 Joystick 1 X    | A0   |
| 🎮 Joystick 1 Y    | A1   |
| 🎮 Joystick 2 X    | A2   |
| 🎮 Joystick 2 Y    | A3   |
| 🦶 Pedal 1 (Z)     | A4   |
| 🦶 Pedal 2 (Rz)    | A5   |
| 🔘 Button 1        | D2   |
| 🔘 Button 2        | D3   |
| 🔘 Button 3        | D4   |
| 🔘 Button 4        | D5   |
| 🔘 Button 5        | D6   |
| 🔘 Button 6        | D7   |
| 🔘 Button 7        | D8   |
| 🔘 Button 8        | D9   |
| 🔘 Button 9        | D10  |
| 🔘 Button 10       | D11  |
| 🔘 Button 11       | D12  |
| 🔘 Button 12       | D13  |

> **Note:** Unused analog pins should be tied to **GND** or configured with `INPUT_PULLUP` to avoid floating values.

---

## ⚡ Wiring Overview
- **Joysticks:**  
  - Joystick 1 → A0 (X), A1 (Y)  
  - Joystick 2 → A2 (Rx), A3 (Ry)  
  - Pot middle = signal, sides = GND & +5V  

- **Pedals:**  
  - Pedal 1 → A4 (Z)  
  - Pedal 2 → A5 (Rz)  
  - Pot middle = signal, sides = GND & +5V  

- **Buttons:**  
  - One side to pin (D2–D13)  
  - Other side to GND  
  - `INPUT_PULLUP` enabled — pressed = LOW  

---

## 💻 Software Setup

### 1️⃣ Install Arduino IDE & Joystick Library

# Arduino IDE download: https://www.arduino.cc/en/software
In Arduino IDE → Sketch → Include Library → Manage Libraries

Search "Joystick" by Matthew Heironimus → Install

2️⃣ Select Board & Port
Tools → Board → Arduino Leonardo

Tools → Port → (Your Leonardo)

3️⃣ Upload Code
Open main.ino

Verify & Upload

Unplug/replug USB after first upload to enumerate HID

🎛️ Customization
Invert Axis:

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
Edit AXIS_PINS[] in the code.

Modify Button Count:
Edit buttonPins[] and NUM_BUTTONS.

🐧 Linux Testing
CLI (jstest):

```
bash
Copy
Edit
sudo apt install joystick
sudo modprobe joydev
jstest /dev/input/js0
GUI (jstest-gtk):

bash
Copy
Edit
sudo apt install jstest-gtk
jstest-gtk
Web Tester:
https://gamepad-tester.com (Chrome/Firefox)
```
🪟 Windows Testing
Press Win + R, type:

Copy
Edit
joy.cpl
Select your Leonardo controller

Move joysticks / press buttons to verify mapping

⚠️ Known Issues
Floating analog pins cause random movement → tie to GND or enable pull-up.

Linux axis/button mapping order may differ from Windows.

HID mapping changes require unplug/replug after flashing.





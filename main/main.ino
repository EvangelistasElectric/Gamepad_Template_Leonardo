/*
  ================================================================
   Project: Leonardo HID Joystick & Pedal Controller
   Author:  Matthew Evangelista (Evangelista's Electric)
   License: Evangelista's Electric Non-Commercial Open Source License (EENOSL)
   Date:    2025-08-13
  ================================================================

   Description:
   This firmware turns an Arduino Leonardo (ATmega32U4) into a
   USB HID-compliant game controller featuring:
     - Two analog joysticks (4 axes total: X, Y, Rx, Ry)
     - Two analog pedals (Z, Rz)
     - 12 digital buttons (mapped to configurable pins)
   It is designed for simulator controls and custom input devices.

   Features:
     • Analog oversampling for smoother axis readings
     • Deadzone filtering to eliminate small unwanted movements
     • Individual axis inversion via INVERT[] array
     • Cross-platform HID support (Windows, Linux, Mac)
     • Auto HID enumeration delay for stable startup

   Pin Mapping:
     A0  → Joystick 1 X Axis
     A1  → Joystick 1 Y Axis
     A2  → Joystick 2 X Axis (Rx)
     A3  → Joystick 2 Y Axis (Ry)
     A4  → Pedal 1 (Z Axis)
     A5  → Pedal 2 (Rz Axis)
     D2–D13 → Buttons 1–12 (INPUT_PULLUP, active LOW)

   Dependencies:
     - Arduino Joystick Library by Matthew Heironimus
       https://github.com/MHeironimus/ArduinoJoystickLibrary

   Notes:
     - Unused analog pins should be tied to GND or set to INPUT_PULLUP
       to prevent floating values.
     - HID mapping changes may require unplug/replug after upload.
     - Ensure 'Arduino Leonardo' is selected in Tools → Board.

  ================================================================
*/

#include <Arduino.h>
#include <Arduino.h>
#include <Joystick.h>

/*
  Leonardo HID Gamepad layout
  - A0/A1:   Stick 1 -> X, Y
  - A2/A3:   Stick 2 -> Rx, Ry
  - A4:      Pedal 1 -> Z
  - A5:      Pedal 2 -> Rz
  - Buttons: all other GPIO listed in buttonPins[] (INPUT_PULLUP, active LOW)
*/

// ---------- Analog config ----------
const uint8_t AXIS_PINS[6] = {A0, A1, A2, A3, A4, A5}; // X,Y,Rx,Ry,Z,Rz (order below matches sets)
bool INVERT[6] = {true, true, false, false, false, false};

const int16_t  AXIS_MIN  = -32767;
const int16_t  AXIS_MAX  =  32767;
const uint16_t DEADZONE  = 16; // around center on 0..1023 scale
const uint8_t  SAMPLES   = 8;  // analog oversampling

// ---------- Buttons (exclude A0..A5 because they’re axes; avoid D0/D1 UART) ----------
const uint8_t buttonPins[] = {
  // D2..D13 are fair game on Leonardo
  2,3,4,5,6,7,8,9,10,11,12,13
};
#define NUM_BUTTONS (sizeof(buttonPins) / sizeof(buttonPins[0]))

// ---------- HID setup: enable X,Y,Z,Rx,Ry,Rz (6 axes), no hats ----------
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
  NUM_BUTTONS, 0,      // buttons, hats
  true, true, true,    // X, Y, Z
  true, true, true,    // Rx, Ry, Rz
  false, false, false, // rudder, throttle, accelerator
  false, false         // brake, steering
);

// ---------- Helpers ----------
static inline int16_t mapWithDeadzone(uint16_t v){
  const int16_t center = 512;
  if (v > center - DEADZONE && v < center + DEADZONE) return 0;

  if (v < center) {
    return (int16_t)map(v, 0, center - DEADZONE, AXIS_MIN, 0);
  } else {
    return (int16_t)map(v, center + DEADZONE, 1023, 0, AXIS_MAX);
  }
}

static inline uint16_t readAveraged(uint8_t pin){
  uint32_t sum = 0;
  for(uint8_t i = 0; i < SAMPLES; i++){
    sum += analogRead(pin);
  }
  return (uint16_t)(sum / SAMPLES);
}

void setup() {
  // Buttons
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // Axis ranges
  Joystick.setXAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setYAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setZAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setRxAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setRyAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setRzAxisRange(AXIS_MIN, AXIS_MAX);

  // Allow USB to enumerate before we start sending HID reports
  delay(1500);
  Joystick.begin();
}

void loop() {
  // Read axes (A0,A1 -> X,Y) (A2,A3 -> Rx,Ry) (A4 -> Z) (A5 -> Rz)
  int16_t v[6];
  for (uint8_t i = 0; i < 6; i++) {
    v[i] = mapWithDeadzone(readAveraged(AXIS_PINS[i]));
    if (INVERT[i]) v[i] = -v[i];
  }

  Joystick.setXAxis(v[0]); // A0
  Joystick.setYAxis(v[1]); // A1
  Joystick.setRxAxis(v[2]); // A2
  Joystick.setRyAxis(v[3]); // A3
  Joystick.setZAxis(v[4]);  // A4 (Pedal 1)
  Joystick.setRzAxis(v[5]); // A5 (Pedal 2)

  // Buttons (active LOW)
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    Joystick.setButton(i, digitalRead(buttonPins[i]) == LOW);
  }

  Joystick.sendState();
  delay(2);
}

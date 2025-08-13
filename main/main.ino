#include<Arduino.h>
#include<Joystick.h>

//------ Config ------
#define NUM_BUTTONS 8
const uint8_t buttonPins[NUM_BUTTONS] = {2,3,4,5,6,7,8,9};

// Three 2-axis sticks -> 6 axes on A0 - A5
const uint8_t AXIS_PINS[6] = {A0, A1, A2, A3, A4, A5}; // X,Y,Z,Rx,Ry,Rz in order
bool INVERT[6] = {false, false, false, false, false, false};

const int16_t AXIS_MIN = -32767;
const int16_t AXIS_MAX =  32767;
const uint16_t DEADZONE = 16; // around center on 0..1023 scale 
const uint8_t SAMPLES = 8;    // analog oversampling

// HID: enable X, Y, Z, Rx, Ry, Rz
Joystick_ Joystick(
  JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_GAMEPAD,
  NUM_BUTTONS, 0,      // buttons, hats
  true, true, true,    // X, Y, Z
  true, true, true,    // Rx, Ry, Rz
  false, false, false, // rudder, throttle, accelerator
  false, false         // brake, steering
);

static inline int16_t mapWithDeadzone(uint16_t v){
  const int16_t center = 512;
  if(v > center - DEADZONE && v < center + DEADZONE){
    return 0;
  }
  if(v < center){
    return (int16_t)map(v, 0, center - DEADZONE, AXIS_MIN, 0);
  }else{
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
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) pinMode(buttonPins[i], INPUT_PULLUP);

  Joystick.setXAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setYAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setZAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setRxAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setRyAxisRange(AXIS_MIN, AXIS_MAX);
  Joystick.setRzAxisRange(AXIS_MIN, AXIS_MAX);

  delay(1500);   // allow USB to enumerate
  Joystick.begin();
}

void loop() {
  // Axes: X,Y,Z,Rx,Ry,Rz
  int16_t v[6];
  for (uint8_t i = 0; i < 6; i++) {
    v[i] = mapWithDeadzone(readAveraged(AXIS_PINS[i]));
    if (INVERT[i]) v[i] = -v[i];
  }
  Joystick.setXAxis(v[0]);
  Joystick.setYAxis(v[1]);
  Joystick.setZAxis(v[2]);
  Joystick.setRxAxis(v[3]);
  Joystick.setRyAxis(v[4]);
  Joystick.setRzAxis(v[5]);

  // Buttons
  for (uint8_t i = 0; i < NUM_BUTTONS; i++) {
    Joystick.setButton(i, digitalRead(buttonPins[i]) == LOW);
  }

  Joystick.sendState();
  delay(2);
}

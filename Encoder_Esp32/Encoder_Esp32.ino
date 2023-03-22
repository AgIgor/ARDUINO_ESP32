#include <Arduino.h>
#include <RotaryEncoder.h>

#define PIN_IN1 18
#define PIN_IN2 19

#define ROTARYSTEPS 1
#define ROTARYMIN 0
#define ROTARYMAX 9

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR0);
int lastPos = -1;

void setup(){
  Serial.begin(115200);
  while (! Serial);
  Serial.println("LimitedRotator example for the RotaryEncoder library.");
  encoder.setPosition(10 / ROTARYSTEPS); // start with the value of 10.
} // setup()

void loop(){
  encoder.tick();
  int newPos = encoder.getPosition() * ROTARYSTEPS;

  if (newPos < ROTARYMIN) {
    encoder.setPosition(ROTARYMIN / ROTARYSTEPS);
    newPos = ROTARYMIN;

  } else if (newPos > ROTARYMAX) {
    encoder.setPosition(ROTARYMAX / ROTARYSTEPS);
    newPos = ROTARYMAX;
  } // if

  if (lastPos != newPos) {
    Serial.print(newPos);
    Serial.println();
    lastPos = newPos;
  } // if
} // loop ()

// The End


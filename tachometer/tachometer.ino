#include "measurement.h"
#include "output.h"
#include "simulation.h"

const int PIN_OUT = 13;

Measurement measurement;
Output output(PIN_OUT);

Simulation sim(&measurement);

void setup() {
  digitalWrite(PIN_OUT, LOW);
  pinMode(PIN_OUT, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), onWheelTurned, RISING);

  sim.changeSpeedTo(15, 15);
  
  sim.start();
}

void onWheelTurned() {
  measurement.wheelTurned(millis());
}

boolean shouldSwapOutput() {
  const int MAX_PULSE_LENGTH = 50;
  
  const long now = millis();
  const int expectedPulseLength = measurement.outputPulseLength();

  const boolean pulseTooLongToEmit = expectedPulseLength > MAX_PULSE_LENGTH;
  const boolean currentStateForLongEnough = now - output.notChangedSince() >= expectedPulseLength;

  return !pulseTooLongToEmit && currentStateForLongEnough;
}

void loop() {
  if (shouldSwapOutput()) {
    output.swap();
  }
  measurement.wheelNotTurned(millis());
  sim.runStep();
}

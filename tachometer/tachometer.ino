#include "measurement.h"
#include "output.h"
#include "simulation.h"

const int PIN_OUT = 13;

Measurement measurement;
Output output(PIN_OUT);

#ifdef SIMULATION
Simulation sim(&measurement);
#endif

void setup() {
  digitalWrite(PIN_OUT, LOW);
  pinMode(PIN_OUT, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(2), onWheelTurned, RISING);

#ifdef SIMULATION
  sim.changeSpeedTo(30, 20);
  sim.changeSpeedTo(10, 10);
  sim.wait(10);
  
  sim.start();
#endif
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
#ifdef SIMULATION
  sim.runStep();
#endif
}

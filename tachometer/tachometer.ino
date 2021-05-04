const int INTERRUPT_PIN = 2;
const int PIN_OUT = 13;

const int MAX_PULSE_LENGTH = 50;
const long MAX_LAST_DIFF = 999999;
const int DEBOUNCE_TIME = 150;

volatile long initialTime = millis();

volatile long lastWheelTurnAt = 0;
volatile long lastDiff = MAX_LAST_DIFF;

void setup() {
  digitalWrite(PIN_OUT, LOW);
  pinMode(PIN_OUT, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), onWheelTurned, RISING);
}

void onWheelTurned() {
  long now = millis();
  if (now > lastWheelTurnAt + DEBOUNCE_TIME) {
    lastDiff = now - lastWheelTurnAt;
    lastWheelTurnAt = now;
  }
}

inline int pulseLength() {
  return lastDiff / 80; // 80.46
}

long currentOutput = LOW;
long highSince = 0;
long lowSince = 0;

inline void setLow() {
  if (currentOutput != LOW) {
    digitalWrite(PIN_OUT, LOW);
    currentOutput = LOW;
    lowSince = millis();
  }
}

inline void setHigh() {
  if (currentOutput != HIGH) {
    digitalWrite(PIN_OUT, HIGH);
    currentOutput = HIGH;
    highSince = millis();
  }
}

inline void swapOutput() {
  if (currentOutput == LOW) {
    setHigh();
  } else {
    setLow();
  }
}

inline long currentOutputSince() {
  if (currentOutput == LOW) {
    return lowSince;
  } else {
    return highSince;
  }
}

inline boolean shouldSwapOutput() {
  const long now = millis();
  const int expectedPulseLength = pulseLength();
  
  const boolean pulseTooLongToEmit = expectedPulseLength > MAX_PULSE_LENGTH;
  const boolean currentStateForLongEnough = now - currentOutputSince() >= expectedPulseLength;

  return !pulseTooLongToEmit && currentStateForLongEnough;
}

void loop() {
  if (shouldSwapOutput()) {
    swapOutput();
  }
}

#ifndef MEASUREMENT_H
#define MEASUREMENT_H

class Measurement {
  public:
    Measurement() {
      this->lastWheelTurnAt = 0;
      this->lastDiff = 999999;
    }
  
    void wheelTurned(long now) {
      if (now > this->lastWheelTurnAt + debounceTime) {
        this->lastDiff = now - this->lastWheelTurnAt;
        this->lastWheelTurnAt = now;
      }
    }

    void wheelNotTurned(long now) {
      if (now - lastWheelTurnAt > lastDiff) {
        this->lastDiff = now - this->lastWheelTurnAt;
      }
    }

    int outputPulseLength() {
      return lastDiff / 80; // 80.46
    }
  
  private:
    volatile long lastWheelTurnAt;
    volatile long lastDiff;
    const int debounceTime = 150;
};

#endif

#ifndef SIMULATION_H
#define SIMULATION_H

#include "measurement.h"

class Simulation {
  public:
    Simulation(Measurement* measurement) {
      this->measurement = measurement;
      this->program[0] = 0;
      this->currentDefinition = this->program;
      this->currentExecution = &this->program[1];
    }
  
    void changeSpeedTo(int kph, int seconds) {
      int v0 = this->toSpeedKph(*this->currentDefinition);
      double v0_m_per_sec = this->toMetersPerSec(v0);
      double a = this->accelerationMetersPerSecSq(v0, kph, seconds);

      int timeRolled = 0;

      while (timeRolled < seconds * 1000) {
        int currentDelay = this->acceleratedDelay(a, v0_m_per_sec);
        this->addStep(currentDelay);
        timeRolled += currentDelay;
        v0_m_per_sec += a * currentDelay / 1000.0;
      }
    }

    void wait(int seconds) {
      int currentDelay = *this->currentDefinition;
      int waitingTime = 0;
      
      while(waitingTime < seconds) {
        waitingTime += currentDelay;
        this->addStep(currentDelay);
      }
    }

    void start() {
      this->lastStepAt = millis();
    }

    void runStep() {
      if (this->currentExecution >= this->currentDefinition) {
          return;
      }
      long now = millis();
      
      if (now >= this->lastStepAt + *this->currentExecution) {
        this->measurement->wheelTurned(now);
        this->lastStepAt = now;
        this->currentExecution++;
      }
    }

  private:
    long toDelayMillis(long kph) {
      long val = kph != 0 ? 8046 / kph : 0;
      return val;
    }

    long toSpeedKph(long delayMillis) {
      return delayMillis != 0 ? 8046 / delayMillis : 0;
    }

    void addStep(int delayMillis) {
      this->currentDefinition++;
      *this->currentDefinition = delayMillis;
    }

    double accelerationMetersPerSecSq(int v0_kph, int vmax_kph, int seconds) {
      return (this->toMetersPerSec(vmax_kph) - this->toMetersPerSec(v0_kph)) / seconds;
    }

    double toMetersPerSec(double kph) {
      return kph * 5.0 / 18.0;
    }

    int acceleratedDelay(double a, double v0) {
      double R = 0.3556;
      return (int) (((sqrt(v0 * v0 + 4 * PI * a * R) - v0) / a) * 1000.0);
    }

    const int stepSize = 200;
  
    long lastStepAt;

    int program[600];
    int programSize;
 
    int* currentDefinition;
    int* currentExecution;
    
    Measurement* measurement;
};

#endif

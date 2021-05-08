#ifndef OUTPUT_H
#define OUTPUT_H

class Output {
  public:
    Output(int pinOut) {
      this->pinOut = pinOut;
      this->currentOutput = LOW;
      this->highSince = 0;
      this->lowSince = 0;
    }

    void swap() {
      if (currentOutput == LOW) {
        this->setHigh();
      } else {
        this->setLow();
      }
    }

    long notChangedSince() {
      return currentOutput == LOW ? lowSince : highSince;
    }
  
  private:
    void setLow() {
      if (this->currentOutput != LOW) {
        digitalWrite(this->pinOut, LOW);
        this->currentOutput = LOW;
        this->lowSince = millis();
      }
    }
    
    void setHigh() {
      if (this->currentOutput != HIGH) {
        digitalWrite(this->pinOut, HIGH);
        this->currentOutput = HIGH;
        this->highSince = millis();
      }
    }

    int pinOut;
    int currentOutput;
    long highSince;
    long lowSince;
};

#endif

#ifndef CABLEDETECTOR_H
#define CABLEDETECTOR_H

#include <Arduino.h>

class CableDetector {
  private:
    const byte pin_cable_sensor = A2;
    const byte pin_buzzer = 10; 
    int cable_value; 
  public:
    CableDetector();
    void init();
    void detectCable();
    int getCableValue(); 
    void beep(int frequency, int duration); 
};

#endif

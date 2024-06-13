#include "CableDetector.h"

CableDetector::CableDetector() {}

void CableDetector::init() {
  pinMode(pin_cable_sensor, INPUT);
  pinMode(pin_buzzer, OUTPUT); // Buzzer pinini çıkış olarak ayarla
}

void CableDetector::detectCable() {
  cable_value = analogRead(pin_cable_sensor);
}

int CableDetector::getCableValue() {
  return cable_value;
}

void CableDetector::beep(int frequency, int duration) {
  digitalWrite(pin_buzzer, HIGH);
  delay(duration);
  digitalWrite(pin_buzzer, LOW);
}

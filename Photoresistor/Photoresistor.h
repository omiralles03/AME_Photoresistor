#ifndef PHOTORESISTOR_H
#define PHOTORESISTOR_H

#include "mbed.h"

class Photoresistor {
public:
  Photoresistor(PinName pin);

  float calculateLux();

private:
  AnalogIn _pin;

  const float _rl = 10000.0f;
  const float _vref = 3.3f;
  const float _luxRel = 500.0f;
};

#endif

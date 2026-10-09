#include "Photoresistor.h"

Photoresistor::Photoresistor(PinName pin) : _pin(pin) {}

// Lux = [((Vref * luxRel) * Vout) - luxRel] / Rl
float Photoresistor::calculateLux() {
  // ADCres = 65535 if you are using read_u16(), or 1.0 if you are using
  // read()
  float ADCres = 1.0f;
  float vout = _pin.read();

  float vCalc = ((_vref * _luxRel) * vout) - _luxRel;

  if (vCalc < 0)
    return 0.0f;
  else
    return vCalc / _rl;
}

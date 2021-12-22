/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_SINGLE_COLOR_H
#define MODE_SINGLE_COLOR_H

#include "Mode_Rainbow.h"

class Mode_SingleColor: public Mode_Rainbow {
public:
  Mode_SingleColor(int eepCont): Mode_Rainbow(eepCont) {};
  void display(Ytani_NeoPixel *leds) override;
};
#endif // MODE_SINGLE_COLOR_H
/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_SINGLE_COLOR_H
#define MODE_SINGLE_COLOR_H

#include "Mode_Rainbow.h"

class Mode_SingleColor: public Mode_Rainbow {
public:
  void display(Ytani_NeoPixel *leds) override;
protected:
  int __eep_continuous = 0x12;
};
#endif // MODE_SINGLE_COLOR_H

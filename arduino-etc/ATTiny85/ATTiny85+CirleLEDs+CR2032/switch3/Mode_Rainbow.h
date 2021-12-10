/**
 * Copyright (c) Yoichi Tanibayashi
 */
#ifndef MODE_RAINBOW_H
#define MODE_RAINBOW_H

#include "common.h"
#include "ModeBase.h"

class Mode_Rainbow: public ModeBase {
public:
  const uint8_t BRIGHTNESS_MAX = 64;
  uint8_t       CurBr = BRIGHTNESS_MAX >> 2;

  Mode_Rainbow();

  void loop(Ytani_NeoPixel *leds, Button *btn);
  boolean btn_loop_hdr(Ytani_NeoPixel *leds, Button *btn);
  virtual void display(Ytani_NeoPixel *leds);

  void incHS();

protected:
  int           _cur_hs = 0;
  unsigned long _continus = 0;
};
#endif // MODE_RAINBOW_H

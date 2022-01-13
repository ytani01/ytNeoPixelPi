/**
 * Copyright (c) 2021 Yoichi Tanibayashi
 */
#include <EEPROM.h>
#include "Button.h"
#include "Ytani_NeoPixel.h"
#include "Mode_Rainbow.h"
#include "Mode_SingleColor.h"
#include "Mode_Belt1.h"

const unsigned long LOOP_DELAY = 0;  // ms
const unsigned long DEBOUNCE   = 50;  // ms

const uint8_t PIN_BTN = 3;
const uint8_t PIN_LEDS  = 4;
const uint8_t LEDS_N   = 10;
const uint8_t BRIGHTNESS_MAX = 255;
uint8_t CurBr = BRIGHTNESS_MAX / 4;

Ytani_NeoPixel   *Leds;
Button Btn(PIN_BTN, "Button");

int eepMode = 0; // uint8_t (1 byte)
int eepBr   = eepMode + 1; // uint8_t (1 byte)
int eepRainbowCont = eepBr + 1; // unsigned long (4 bytes)
int eepSingleColorCont = eepRainbowCont + 4; // unsigned long (4 bytes)
int eepBelt1Cont = eepSingleColorCont + 4; // unsigned long (4 bytes)
int eepRainbowSat = eepBelt1Cont + 4; // uint8_t (1 bytes)
int eepSingleColorSat = eepRainbowSat + 1; // uint8_t (1 bytes)

Mode_SingleColor mode_single_color(eepSingleColorCont, eepSingleColorSat);
Mode_Rainbow     mode_rainbow(eepRainbowCont, eepRainbowSat);
Mode_Belt1       mode_belt1(eepBelt1Cont, false);

ModeBase* Mode[] =
  {
   &mode_single_color,
   &mode_rainbow,
   &mode_belt1
  };
const int ModeN = sizeof(Mode) / sizeof(Mode[0]);
uint8_t CurMode = 0;

/**
 * ATTiny85 の割り込みルーチン(固定)
 *
 *  PCINT0_vect: ピン変化割り込み要求0(固定)
 */
ISR(PCINT0_vect) {
  btn_intr_hdr(); 
}

/**
 *
 */
void btn_intr_hdr() {
  static unsigned long prev_ms = 0;
  unsigned long        cur_ms = millis();

  if ( cur_ms - prev_ms > DEBOUNCE ) {
    prev_ms = cur_ms;
  
    (void)Btn.get();
  }
} // btn_intr_hdr()

/**
 *
 */
void btn_loop_hdr() {
  if ( Mode[CurMode]->btn_loop_hdr(Leds, &Btn) ) {
    return;
  }

  int n = Btn.get_click_count();

  if ( n == 2 ) {
    CurBr /= 4;
    if ( CurBr == 0 ) {
      CurBr = BRIGHTNESS_MAX;
    }
    EEPROM.put(eepBr, CurBr);
    return;
  }

  if ( n > 2 ) {
    CurMode = (CurMode + 1) % ModeN;
    EEPROM.put(eepMode, CurMode);
    return;
  }
} // btn_loop_hdr()

/**
 *
 */
void setup() {
  randomSeed(analogRead(0));

  Leds = new Ytani_NeoPixel(LEDS_N, PIN_LEDS);
  Leds->clear();
  Leds->setColor(0, 0x0000ff);
  Leds->show();

  EEPROM.get(eepMode, CurMode);
  if ( CurMode >= ModeN ) {
    CurMode = 0;
  }

  EEPROM.get(eepBr, CurBr);
  if ( CurBr == 0 || CurBr > BRIGHTNESS_MAX ) {
    CurBr = BRIGHTNESS_MAX / 4;
  }

  // 割込設定
  GIMSK |= (1 << PCIE);  // PCINT割り込み有効
  PCMSK = (1 << PCINT3); // Pin3の割り込み許可
  interrupts();

  delay(1000);
} // setup()

/**
 *
 */
void loop() {
  if ( Btn.get() ) {
    noInterrupts();
    btn_loop_hdr();
    interrupts();
  }

  Mode[CurMode]->loop(Leds, &Btn);
  Leds->show();
  interrupts();

  delay(LOOP_DELAY);
} // loop()

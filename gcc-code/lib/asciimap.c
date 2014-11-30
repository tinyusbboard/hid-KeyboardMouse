/*
 * asciimap.c
 * 
 * This file implements ASCII symbol to keystroke-
 * report mapping.
 * Currently implemented keyboard layouts are:
 * 	# en_us (41)
 * 	# de (10)
 * 
 * The default layout always will be en_us.
 * Unknown symbols aren't mapped. 
 * 
 * by Stephan Baerwolf (stephan@matrixstorm.com), Schwansee 2014
 * for http://matrixstorm.com/avr/tinyusbboard/
 * (please contact me at least before commercial use)
 */

#define __ASCIIMAP_C_7b57b2239b7d418b8332be68649a9120 1

#include "asciimap.h"

#ifndef USB_CFG_HID_NOKEYBOARD

uint8_t asciitokeyreport(uint8_t key, keyboard_report_t *report) {
  uint8_t result = 1;
  
  if (key == ' ') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_spacebar;
  } else if (key == '\n') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_return;
  } else if (key == '\t') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_tab;
#if ASCIIMAP_LAYOUT == ASCIIMAP_LAYOUT_DE
  } else if (key == 'z') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_y;
  } else if (key == 'Z') {
    report->modifier = _BV(HIDKEYBOARD_MODBIT_LEFT_SHIFT);
    report->keycode[0] = HIDKEYBOARD_KEYUSE_y;
  } else if (key == 'y') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_z;
  } else if (key == 'Y') {
    report->modifier = _BV(HIDKEYBOARD_MODBIT_LEFT_SHIFT);
    report->keycode[0] = HIDKEYBOARD_KEYUSE_z;
#endif
  } else  if ((key >= 'a') && (key <= 'z')) {
    report->keycode[0] = (key-'a')+HIDKEYBOARD_KEYUSE_a;
  } else if ((key >= 'A') && (key <= 'Z')) {
    report->modifier = _BV(HIDKEYBOARD_MODBIT_LEFT_SHIFT);
    report->keycode[0] = (key-'A')+HIDKEYBOARD_KEYUSE_a;
  } else if ((key >= '1') && (key <= '9')) {
    report->keycode[0] = (key-'1')+HIDKEYBOARD_KEYUSE_1;
  } else if (key == '0') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_0;
  } else if (key == '@') {
#if ASCIIMAP_LAYOUT == ASCIIMAP_LAYOUT_DE
    report->modifier = _BV(HIDKEYBOARD_MODBIT_RIGHT_ALT);
    report->keycode[0] = HIDKEYBOARD_KEYUSE_q;
#else
    report->modifier = _BV(HIDKEYBOARD_MODBIT_LEFT_SHIFT);
    report->keycode[0] = HIDKEYBOARD_KEYUSE_2;
#endif
  } else if (key == ':') {
#if ASCIIMAP_LAYOUT == ASCIIMAP_LAYOUT_DE
    report->modifier = _BV(HIDKEYBOARD_MODBIT_LEFT_SHIFT);
    report->keycode[0] = HIDKEYBOARD_KEYUSE_period;
#else
    report->modifier = _BV(HIDKEYBOARD_MODBIT_LEFT_SHIFT);
    report->keycode[0] = HIDKEYBOARD_KEYUSE_semicolon;
#endif
  } else if (key == ',') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_comma;
  } else if (key == '/') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_KEYPAD_div;
  } else if (key == '*') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_KEYPAD_mul;
  } else if (key == '+') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_KEYPAD_add;
  } else if (key == '-') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_KEYPAD_min;
  } else if (key == '.') {
    report->keycode[0] = HIDKEYBOARD_KEYUSE_period;
  } else result=0;

  return result;
}

#endif

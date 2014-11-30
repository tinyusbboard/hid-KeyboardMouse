#include "VUSBHIDKeyboardMouse.h"

#define LED_NUMLOCK	8
#define LED_CAPSLLOCK	9
#define LED_SCROLLLOCK	5
#define LED_BLINK	3


/* 
 * in order to work without timer, the following value configures timing 
 * please calibrate it, to see the "LED_BLINK" blink 
 * with about 8sec (4sec on and 4sec off)..
 */
#define TIMECTRLTUNE	400



void setup() {
  // disables Arduino's default millisecond counter (it disturbs the USB otherwise)
#ifdef TIMSK
  // older ATmega
  TIMSK &= ~(_BV(TOIE0));
#else
  // newer ATmega
  TIMSK0 &= ~(_BV(TOIE0));
#endif

  // remaining inits...
  pinMode(LED_NUMLOCK, OUTPUT);
  pinMode(LED_CAPSLLOCK, OUTPUT);
  pinMode(LED_SCROLLLOCK, OUTPUT);
  pinMode(LED_BLINK, OUTPUT);
  
  /* start with LED switched on */
  digitalWrite(LED_BLINK, 1);
}

static uint16_t toSecondsCounter	= 0;
static uint16_t timeCalibrationCounter	= 0;
void loop() {
  timeCalibrationCounter++;

  if (timeCalibrationCounter<TIMECTRLTUNE) {
    VUSBHIDKeyboardMouse.update(0);
  } else {
    /* every 4ms */
    timeCalibrationCounter=0;
    VUSBHIDKeyboardMouse.update(1);
    toSecondsCounter++;
  }

  /* the calibrated frequency to measure */
  if (toSecondsCounter >= 1000) {
    toSecondsCounter=0;
    digitalWrite(LED_BLINK, !digitalRead(LED_BLINK));
  }
}



#ifndef USB_CFG_HID_NOKEYBOARD
#ifdef __cplusplus
extern "C"{
#endif
void EVENT_CHANGE_LED_state (void) {
  digitalWrite(LED_NUMLOCK, current_LED_state & _BV(HIDKEYBOARD_LEDBIT_NUM_LOCK));
  digitalWrite(LED_CAPSLLOCK, current_LED_state & _BV(HIDKEYBOARD_LEDBIT_CAPS_LOCK));
  digitalWrite(LED_SCROLLLOCK, current_LED_state & _BV(HIDKEYBOARD_LEDBIT_SCROLL_LOCK));
}
#ifdef __cplusplus
}
#endif
#endif

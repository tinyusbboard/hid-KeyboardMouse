#include "VUSBHIDKeyboardMouse.h"


#define LED_BLINK	3
#define BUTTON_PIN	6

/* 
 * in order to work without timer, the following value configures timing 
 * please calibrate it, to see the "LED_BLINK" blink 
 * with about 8sec (4sec on and 4sec off)..
 */
#define TIMECTRLTUNE	300


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
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(BUTTON_PIN, HIGH);

  /* start with LED switched on */
  pinMode(LED_BLINK, OUTPUT);
  digitalWrite(LED_BLINK, 1);
}




static uint16_t toSecondsCounter	= 0;
static uint16_t timeCalibrationCounter	= 0;
void loop() {
  timeCalibrationCounter++;

  if (timeCalibrationCounter<TIMECTRLTUNE) {
    UsbKeyboard.update(0);
  } else {
    /* every 4ms */
    timeCalibrationCounter=0;
    UsbKeyboard.update(1);
    toSecondsCounter++;
  }

  /* the calibrated frequency to measure */
  if (toSecondsCounter >= 1000) {
    toSecondsCounter=0;
    digitalWrite(LED_BLINK, !digitalRead(LED_BLINK));
  }

  if (digitalRead(BUTTON_PIN) == 0) {
    
    //UsbKeyboard.sendKeyStroke(KEY_B, MOD_GUI_LEFT);
    
    UsbKeyboard.sendKeyStroke(KEY_H);
    UsbKeyboard.sendKeyStroke(KEY_E);
    UsbKeyboard.sendKeyStroke(KEY_L);
    UsbKeyboard.sendKeyStroke(KEY_L);
    UsbKeyboard.sendKeyStroke(KEY_O);

    UsbKeyboard.sendKeyStroke(KEY_SPACE);

    UsbKeyboard.sendKeyStroke(KEY_W);
    UsbKeyboard.sendKeyStroke(KEY_O);
    UsbKeyboard.sendKeyStroke(KEY_R);
    UsbKeyboard.sendKeyStroke(KEY_L);
    UsbKeyboard.sendKeyStroke(KEY_D);
    //UsbKeyboard.sendKeyStroke(KEY_B, MOD_GUI_LEFT);

    UsbKeyboard.sendKeyStroke(KEY_ENTER);
   }
}
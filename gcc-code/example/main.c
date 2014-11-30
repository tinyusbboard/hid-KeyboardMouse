
#include "main.h"
#include "tinyusbboard.h"
#include "hidcore.h"
#include "asciimap.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include "usbdrv.h"
#include "oddebug.h"
#include <util/delay.h>

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifndef USB_CFG_HID_NOKEYBOARD
#	define	LED_NUMLOCK	LED_B
#	define	LED_CAPSLOCK	LED_PWM
#	define	LED_SCROLLLOCK	LED_LEFT
void EVENT_CHANGE_LED_state (void) {
  // NUM LOCK
  if (current_LED_state & _BV(HIDKEYBOARD_LEDBIT_NUM_LOCK))	SET_HIGH(LED_NUMLOCK);
  else								SET_LOW(LED_NUMLOCK);

  // CAPS LOCK
  if (current_LED_state & _BV(HIDKEYBOARD_LEDBIT_CAPS_LOCK))	SET_HIGH(LED_CAPSLOCK);
  else								SET_LOW(LED_CAPSLOCK);

  // SCROLL LOCK
  if (current_LED_state & _BV(HIDKEYBOARD_LEDBIT_SCROLL_LOCK))	SET_HIGH(LED_SCROLLLOCK);
  else								SET_LOW(LED_SCROLLLOCK);
}
#endif
 

int main(void) {
  uint8_t zahleins = 1;
  uint8_t zahlnull = 0;
  uint16_t tmp	   = 0;
  uint16_t counter = 0;

  bool     pressed = false;

  /* confige peripherals */
  CFG_PULLUP(BUTTON_PROG);
  CFG_OUTPUT(LED_B);
  CFG_OUTPUT(LED_PWM);
  CFG_OUTPUT(LED_LEFT);
  CFG_OUTPUT(LED_RIGHT);

  hidInit();
  usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */

  /* usb disconnect delay */
  _delay_ms(300);
  wdt_reset();

  /* connect the usb */
  usbDeviceConnect();
  usbInit();
  sei();



  for(;;) {
    wdt_reset();

#if (!(defined(USB_CFG_HID_NOMOUSE))) && (defined(USB_CFG_HID_WHEELMOUSE))
    if (IS_PRESSED(BUTTON_PROG)) {
      if /*(!(pressed))*/ (true) {
	if (!(mouse_report_dirty)) {
	  pressed=true;
	  mouse_report_clear(&current_mouse_report);
	  current_mouse_report.displacement[mouse_report_displacement_wheel]=+1;
	  mouse_report_dirty |= 0x2;
	}
      }
    } else {
      if (pressed) {
	if (!(mouse_report_dirty)) {
	  pressed=false;
	  mouse_report_clear(&current_mouse_report);
	  mouse_report_dirty |= 0x2;
	}
      }
    }
#endif
    
    counter++;
#define counterthreshold 555 /* calibrated to about 4ms - LED_RIGHT should flash every 4sec then */
    if (counter >= counterthreshold) {
      counter-=counterthreshold;
      hidPoll(&zahleins);
      tmp++;
#define tmpthreshold 1000
      if (tmp >= tmpthreshold) {
	tmp-=tmpthreshold;
	TOGGLE(LED_RIGHT);
      }
    } else hidPoll(&zahlnull);

  }
  return 0;
}

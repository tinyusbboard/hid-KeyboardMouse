/*
 * Idea from: http://code.rancidbacon.com/ProjectLogArduinoUSB
 * (Interfaces are kept nearly the same.)
 *
 * by Stephan Baerwolf (stephan@matrixstorm.com), Schwansee 2014
 * for http://matrixstorm.com/avr/tinyusbboard/
 * (please contact me at least before commercial use)
 */

#ifndef __VUSBHIDKEYBOARDMOUSE_H_abe53fffc7524ee19e61f5f3305779a2
#define __VUSBHIDKEYBOARDMOUSE_H_abe53fffc7524ee19e61f5f3305779a2 1

#ifdef USB_CFG_HID_NOKEYBOARD
#	if (USB_CFG_HID_NOKEYBOARD <= 0)
#		undef USB_CFG_HID_NOKEYBOARD
#	endif
#endif

#ifdef USB_CFG_HID_NOMOUSE
#	if (USB_CFG_HID_NOMOUSE <= 0)
#		undef USB_CFG_HID_NOMOUSE
#	endif
#endif

#ifdef __cplusplus
extern "C"{
#endif
#	include "asciimap.h"
#	include "hidcore.h"
#ifdef __cplusplus
}
#endif

#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

#include <util/delay.h>

#include <stdint.h>
#include <string.h>

typedef uint8_t byte;

#ifndef USB_CFG_HID_NOKEYBOARD
#	define MOD_CONTROL_LEFT    (_BV(HIDKEYBOARD_MODBIT_LEFT_CTRL))
#	define MOD_SHIFT_LEFT      (_BV(HIDKEYBOARD_MODBIT_LEFT_SHIFT))
#	define MOD_ALT_LEFT        (_BV(HIDKEYBOARD_MODBIT_LEFT_ALT))
#	define MOD_GUI_LEFT        (_BV(HIDKEYBOARD_MODBIT_LEFT_GUI))
#	define MOD_CONTROL_RIGHT   (_BV(HIDKEYBOARD_MODBIT_RIGHT_CTRL))
#	define MOD_SHIFT_RIGHT     (_BV(HIDKEYBOARD_MODBIT_RIGHT_SHIFT))
#	define MOD_ALT_RIGHT       (_BV(HIDKEYBOARD_MODBIT_RIGHT_ALT))
#	define MOD_GUI_RIGHT       (_BV(HIDKEYBOARD_MODBIT_RIGHT_GUI))

#	define KEY_A       HIDKEYBOARD_KEYUSE_a
#	define KEY_B       HIDKEYBOARD_KEYUSE_b
#	define KEY_C       HIDKEYBOARD_KEYUSE_c
#	define KEY_D       HIDKEYBOARD_KEYUSE_d
#	define KEY_E       HIDKEYBOARD_KEYUSE_e
#	define KEY_F       HIDKEYBOARD_KEYUSE_f
#	define KEY_G       HIDKEYBOARD_KEYUSE_g
#	define KEY_H       HIDKEYBOARD_KEYUSE_h
#	define KEY_I       HIDKEYBOARD_KEYUSE_i
#	define KEY_J       HIDKEYBOARD_KEYUSE_j
#	define KEY_K       HIDKEYBOARD_KEYUSE_k
#	define KEY_L       HIDKEYBOARD_KEYUSE_l
#	define KEY_M       HIDKEYBOARD_KEYUSE_m
#	define KEY_N       HIDKEYBOARD_KEYUSE_n
#	define KEY_O       HIDKEYBOARD_KEYUSE_o
#	define KEY_P       HIDKEYBOARD_KEYUSE_p
#	define KEY_Q       HIDKEYBOARD_KEYUSE_q
#	define KEY_R       HIDKEYBOARD_KEYUSE_r
#	define KEY_S       HIDKEYBOARD_KEYUSE_s
#	define KEY_T       HIDKEYBOARD_KEYUSE_t
#	define KEY_U       HIDKEYBOARD_KEYUSE_u
#	define KEY_V       HIDKEYBOARD_KEYUSE_v
#	define KEY_W       HIDKEYBOARD_KEYUSE_w
#	define KEY_X       HIDKEYBOARD_KEYUSE_x
#	define KEY_Y       HIDKEYBOARD_KEYUSE_y
#	define KEY_Z       HIDKEYBOARD_KEYUSE_z
#	define KEY_1       HIDKEYBOARD_KEYUSE_1
#	define KEY_2       HIDKEYBOARD_KEYUSE_2
#	define KEY_3       HIDKEYBOARD_KEYUSE_3
#	define KEY_4       HIDKEYBOARD_KEYUSE_4
#	define KEY_5       HIDKEYBOARD_KEYUSE_5
#	define KEY_6       HIDKEYBOARD_KEYUSE_6
#	define KEY_7       HIDKEYBOARD_KEYUSE_7
#	define KEY_8       HIDKEYBOARD_KEYUSE_8
#	define KEY_9       HIDKEYBOARD_KEYUSE_9
#	define KEY_0       HIDKEYBOARD_KEYUSE_0

#	define KEY_ENTER   HIDKEYBOARD_KEYUSE_return

#	define KEY_SPACE   HIDKEYBOARD_KEYUSE_spacebar

#	define KEY_F1      HIDKEYBOARD_KEYUSE_F1
#	define KEY_F2      HIDKEYBOARD_KEYUSE_F2
#	define KEY_F3      HIDKEYBOARD_KEYUSE_F3
#	define KEY_F4      HIDKEYBOARD_KEYUSE_F4
#	define KEY_F5      HIDKEYBOARD_KEYUSE_F5
#	define KEY_F6      HIDKEYBOARD_KEYUSE_F6
#	define KEY_F7      HIDKEYBOARD_KEYUSE_F7
#	define KEY_F8      HIDKEYBOARD_KEYUSE_F8
#	define KEY_F9      HIDKEYBOARD_KEYUSE_F9
#	define KEY_F10     HIDKEYBOARD_KEYUSE_F10
#	define KEY_F11     HIDKEYBOARD_KEYUSE_F11
#	define KEY_F12     HIDKEYBOARD_KEYUSE_F12

#	define KEY_ARROW_LEFT HIDKEYBOARD_KEYUSE_arrow_left
#endif


#ifndef USB_CFG_HID_NOMOUSE
#	define MOD_MOUSE_LEFT		(_BV(mouse_report_buttonbit_left))
#	define MOD_MOUSE_MIDDLE	(_BV(mouse_report_buttonbit_middle))
#	define MOD_MOUSE_RIGHT		(_BV(mouse_report_buttonbit_right))
#endif


class VUSBHIDKeyboardMouseDevice {
 public:
  VUSBHIDKeyboardMouseDevice () {
    uint8_t i;

    hidInit();

    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */

    do {
     i++;
     wdt_reset();
     _delay_ms(1);
     wdt_reset();
    } while (i);

    usbDeviceConnect();
    usbInit();
    sei();
  }


  void update(uint8_t time_since_lastcall_in_four_ms_units) {
    hidPoll(&time_since_lastcall_in_four_ms_units);
  }
  void update() {
    update(0);
  }

  // Keyboard
#ifndef USB_CFG_HID_NOKEYBOARD
  void sendKeyReport(keyboard_report_t *report) {
    while (keyboard_report_dirty) {
      update();
    }

    memcpy(&current_keyboard_report, report, sizeof(keyboard_report_t));
    keyboard_report_dirty|=_BV(1);

    update();
  }
  void sendKeyRelease() {
    keyboard_report_t report;
    keyboard_report_clear(&report);
    keyboard_report_setNoEvent(&report);
    sendKeyReport(&report);
  }
  void sendKeyStroke(byte keyStroke, byte modifiers) {
    keyboard_report_t report;
    keyboard_report_clear(&report);
    
    report.modifier  =modifiers;
    report.keycode[0]=keyStroke;

    sendKeyReport(&report);
    sendKeyRelease();
  }
  void sendKeyStroke(byte keyStroke) {
    sendKeyStroke(keyStroke, 0);
  }

  keyboard_report_t getKeyReportFromChar(byte symbol) {
    keyboard_report_t result;
    keyboard_report_clear(&result);
    asciitokeyreport(symbol, &result);
    return result;
  }
  void sendKey(byte symbol) {
    keyboard_report_t report = getKeyReportFromChar(symbol);
    sendKeyReport(&report);
    sendKeyRelease();
  }
  
  /* for more compatibility with other keyboard libs */
  void write(byte symbol) {
    sendKey(symbol);
  }
#endif


  // Mouse
#ifndef USB_CFG_HID_NOMOUSE
  void sendMouseReport(mouse_report_t *report) {
    while (mouse_report_dirty) {
      update();
    }

    memcpy(&current_mouse_report, report, sizeof(mouse_report_t));
    mouse_report_dirty|=_BV(1);

    update();
  }
  mouse_report_t getMouseReport(int8_t deltaX, int8_t deltaY, uint8_t buttons) {
    mouse_report_t result;
    mouse_report_clear(&result);
    result.buttons					= buttons;
    result.displacement[mouse_report_displacement_X]	= deltaX;
    result.displacement[mouse_report_displacement_Y]	= deltaY;
    return result;
  }
  void sendMouseIdle() {
    mouse_report_t report;
    mouse_report_clear(&report);
    sendMouseReport(&report);
  }
  void sendMouseEvent(int8_t deltaX, int8_t deltaY, uint8_t buttons) {
    if ((deltaX!=0) || (deltaY!=0) || (buttons!=0)) {
      mouse_report_t report = getMouseReport(deltaX, deltaY, buttons);
      sendMouseReport(&report);
    } else sendMouseIdle();
  }

  /* for more compatibility with other mouse libs */
  void move(int8_t deltaX, int8_t deltaY) {
    sendMouseEvent(deltaX, deltaY, 0);
  }
  void click(uint8_t buttons) {
    sendMouseEvent(0, 0, buttons);
  }
#endif
};

#ifndef USB_CFG_HID_NOKEYBOARD
#	define UsbKeyboard				VUSBHIDKeyboardMouse
#endif

#ifndef USB_CFG_HID_NOMOUSE
#	define UsbMouse					VUSBHIDKeyboardMouse
#endif

VUSBHIDKeyboardMouseDevice	VUSBHIDKeyboardMouse =	VUSBHIDKeyboardMouseDevice();

#endif // __VUSBHIDKEYBOARDMOUSE_H_abe53fffc7524ee19e61f5f3305779a2

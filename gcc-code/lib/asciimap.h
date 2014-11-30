/*
 * asciimap.h
 * 
 * This file is the head for ASCII symbol to keystroke-
 * report mapping.
 * 
 * by Stephan Baerwolf (stephan@matrixstorm.com), Schwansee 2014
 * for http://matrixstorm.com/avr/tinyusbboard/
 * (please contact me at least before commercial use)
 */

#ifndef __ASCIIMAP_H_7b57b2239b7d418b8332be68649a9120
#define __ASCIIMAP_H_7b57b2239b7d418b8332be68649a9120 1

#include "hidcore.h"

#ifdef __ASCIIMAP_C_7b57b2239b7d418b8332be68649a9120
#	define ASCIIMAPPUBLIC
#else
#	define ASCIIMAPPUBLIC		extern
#endif


#define ASCIIMAP_LAYOUT_DE 	10
#define ASCIIMAP_LAYOUT_ENUS	41

#ifndef ASCIIMAP_LAYOUT
#	define ASCIIMAP_LAYOUT		ASCIIMAP_LAYOUT_ENUS
#endif

#ifndef USB_CFG_HID_NOKEYBOARD
/*
 * http://www.cygnal.org/ubb/Forum9/HTML/001381.html and also
 * from page 59 of:
 * HID Usage Tables
 * 6/27/01
 * Version 1.11
 * Hut1_11.pdf
 * 
 * 
 * The bitmap of modifier byte is defined in the HID spec.
 * 8.3 Report Format for Array Items (HID1_11.pdf p56)
 * 
 *   bit
 *    0 LEFT CTRL
 *    1 LEFT SHIFT
 *    2 LEFT ALT
 *    3 LEFT GUI
 *    4 RIGHT CTRL
 *    5 RIGHT SHIFT
 *    6 RIGHT ALT
 *    7 RIGHT GUI
 */
#define HIDKEYBOARD_MODBIT_LEFT_CTRL		0
#define HIDKEYBOARD_MODBIT_LEFT_SHIFT		1
#define HIDKEYBOARD_MODBIT_LEFT_ALT		2
#define HIDKEYBOARD_MODBIT_LEFT_GUI		3
#define HIDKEYBOARD_MODBIT_RIGHT_CTRL		4
#define HIDKEYBOARD_MODBIT_RIGHT_SHIFT		5
#define HIDKEYBOARD_MODBIT_RIGHT_ALT		6
#define HIDKEYBOARD_MODBIT_RIGHT_GUI		7


#define HIDKEYBOARD_KEYUSE_a			0x04
#define HIDKEYBOARD_KEYUSE_b			0x05
#define HIDKEYBOARD_KEYUSE_c			0x06
#define HIDKEYBOARD_KEYUSE_d			0x07
#define HIDKEYBOARD_KEYUSE_e			0x08
#define HIDKEYBOARD_KEYUSE_f			0x09
#define HIDKEYBOARD_KEYUSE_g			0x0a
#define HIDKEYBOARD_KEYUSE_h			0x0b
#define HIDKEYBOARD_KEYUSE_i			0x0c
#define HIDKEYBOARD_KEYUSE_j			0x0d
#define HIDKEYBOARD_KEYUSE_k			0x0e
#define HIDKEYBOARD_KEYUSE_l			0x0f
#define HIDKEYBOARD_KEYUSE_m			0x10
#define HIDKEYBOARD_KEYUSE_n			0x11
#define HIDKEYBOARD_KEYUSE_o			0x12
#define HIDKEYBOARD_KEYUSE_p			0x13
#define HIDKEYBOARD_KEYUSE_q			0x14
#define HIDKEYBOARD_KEYUSE_r			0x15
#define HIDKEYBOARD_KEYUSE_s			0x16
#define HIDKEYBOARD_KEYUSE_t			0x17
#define HIDKEYBOARD_KEYUSE_u			0x18
#define HIDKEYBOARD_KEYUSE_v			0x19
#define HIDKEYBOARD_KEYUSE_w			0x1a
#define HIDKEYBOARD_KEYUSE_x			0x1b
#define HIDKEYBOARD_KEYUSE_y			0x1c
#define HIDKEYBOARD_KEYUSE_z			0x1d

#define HIDKEYBOARD_KEYUSE_1			0x1e
#define HIDKEYBOARD_KEYUSE_2			0x1f
#define HIDKEYBOARD_KEYUSE_3			0x20
#define HIDKEYBOARD_KEYUSE_4			0x21
#define HIDKEYBOARD_KEYUSE_5			0x22
#define HIDKEYBOARD_KEYUSE_6			0x23
#define HIDKEYBOARD_KEYUSE_7			0x24
#define HIDKEYBOARD_KEYUSE_8			0x25
#define HIDKEYBOARD_KEYUSE_9			0x26
#define HIDKEYBOARD_KEYUSE_0			0x27

#define HIDKEYBOARD_KEYUSE_return		0x28
#define HIDKEYBOARD_KEYUSE_escape		0x29
#define HIDKEYBOARD_KEYUSE_delete		0x2a
#define HIDKEYBOARD_KEYUSE_tab			0x2b
#define HIDKEYBOARD_KEYUSE_spacebar		0x2c
#define HIDKEYBOARD_KEYUSE_minus		0x2d
#define HIDKEYBOARD_KEYUSE_equal		0x2e
#define HIDKEYBOARD_KEYUSE_bracketopen		0x2f
#define HIDKEYBOARD_KEYUSE_bracketclose		0x30
#define HIDKEYBOARD_KEYUSE_backslash		0x31
#define HIDKEYBOARD_KEYUSE_hashtag		0x32
#define HIDKEYBOARD_KEYUSE_semicolon		0x33
#define HIDKEYBOARD_KEYUSE_apostrophe		0x34
#define HIDKEYBOARD_KEYUSE_graveaccent		0x35
#define HIDKEYBOARD_KEYUSE_comma		0x36
#define HIDKEYBOARD_KEYUSE_period		0x37
#define HIDKEYBOARD_KEYUSE_slash		0x38

#define HIDKEYBOARD_KEYUSE_F1			0x3a
#define HIDKEYBOARD_KEYUSE_F2			0x3b
#define HIDKEYBOARD_KEYUSE_F3			0x3c
#define HIDKEYBOARD_KEYUSE_F4			0x3d
#define HIDKEYBOARD_KEYUSE_F5			0x3e
#define HIDKEYBOARD_KEYUSE_F6			0x3f
#define HIDKEYBOARD_KEYUSE_F7			0x40
#define HIDKEYBOARD_KEYUSE_F8			0x41
#define HIDKEYBOARD_KEYUSE_F9			0x42
#define HIDKEYBOARD_KEYUSE_F10			0x43
#define HIDKEYBOARD_KEYUSE_F11			0x44
#define HIDKEYBOARD_KEYUSE_F12			0x45

#define HIDKEYBOARD_KEYUSE_pageup		0x4b
#define HIDKEYBOARD_KEYUSE_pagedown		0x4e

#define HIDKEYBOARD_KEYUSE_arrow_right		0x4f
#define HIDKEYBOARD_KEYUSE_arrow_left		0x50
#define HIDKEYBOARD_KEYUSE_arrow_down		0x51
#define HIDKEYBOARD_KEYUSE_arrow_up		0x52

#define HIDKEYBOARD_KEYUSE_KEYPAD_div		0x54
#define HIDKEYBOARD_KEYUSE_KEYPAD_mul		0x55
#define HIDKEYBOARD_KEYUSE_KEYPAD_min		0x56
#define HIDKEYBOARD_KEYUSE_KEYPAD_add		0x57
#define HIDKEYBOARD_KEYUSE_KEYPAD_enter		0x58

ASCIIMAPPUBLIC uint8_t asciitokeyreport(uint8_t key, keyboard_report_t *report);
#endif

#endif

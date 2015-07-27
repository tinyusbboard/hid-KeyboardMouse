/*
 * hidcore.h
 * 
 * This file is the head for basic HID keyboard and
 * Mouse devices (at the same time) using V-USB on
 * an Atmel ATmega microcontroller.
 * The device will be HID BOOT v1.1 conform, so it also
 * will work in BIOS and in legacy operating systems.
 * 
 * by Stephan Baerwolf (stephan@matrixstorm.com), Schwansee 2014
 * for http://matrixstorm.com/avr/tinyusbboard/
 * (please contact me at least before commercial use)
 */

#ifndef __HIDCORE_H_7b57b2239b7d418b8332be68649a9120
#define __HIDCORE_H_7b57b2239b7d418b8332be68649a9120 1

#include <stdint.h>

#include <usbdrv.h>
#include <avr/pgmspace.h>



#ifdef __HIDCORE_C_7b57b2239b7d418b8332be68649a9120
#	define	HIDCOREPUBLIC
#else
#	define	HIDCOREPUBLIC		extern
#endif

#ifndef USBIFACE_CLASS_HID
#	define USBIFACE_CLASS_HID		0x03
#endif

#ifndef USBIFACE_SUBCLASS_BOOT
#	define USBIFACE_SUBCLASS_BOOT		0x01
#endif


#ifndef USBIFACE_PROTOCOL_KEYBOARD
#	define USBIFACE_PROTOCOL_KEYBOARD	0x01
#endif
#ifndef USBIFACE_PROTOCOL_MOUSE
#	define USBIFACE_PROTOCOL_MOUSE		0x02
#endif


#ifndef USBIFACE_NO_DESCRIPTOR
#	define USBIFACE_NO_DESCRIPTOR		0x0
#endif


#ifndef USB_CFG_HID_NOKEYBOARD
#	ifndef USBIFACE_INDEX_KEYBOARD
#		define USBIFACE_INDEX_KEYBOARD	0x0
#	endif
#endif


#ifndef USB_CFG_HID_NOMOUSE
#	ifndef USBIFACE_INDEX_MOUSE
#		ifdef USB_CFG_HID_NOKEYBOARD
#			define USBIFACE_INDEX_MOUSE	0x0
#		else
#			define USBIFACE_INDEX_MOUSE	0x1
#		endif
#	endif
#endif



#if (USB_CFG_DESCR_PROPS_CONFIGURATION > 0)

#if (USB_CFG_HAVE_INTRIN_ENDPOINT==0)
#	error USB_CFG_HAVE_INTRIN_ENDPOINT has to be activated when implementing dual HID
#endif

#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
#	if (USB_CFG_HAVE_INTRIN_ENDPOINT3==0)
#		error USB_CFG_HAVE_INTRIN_ENDPOINT3 has to be activated when implementing dual HID
#	endif
#endif

#if (USB_CFG_DESCR_PROPS_HID!=0)
#	error USB_CFG_DESCR_PROPS_HID is ignored when implementing dual HID
#endif

#if (USB_CFG_DESCR_PROPS_HID!=0)
#	error USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH is ignored when implementing dual HID
#endif

#ifdef USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH
#	error USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH must not be used when implementing dual HID
#endif

#ifdef USB_CFG_INTERFACE_CLASS
#	if (USB_CFG_INTERFACE_CLASS!=0)
#		warning USB_CFG_INTERFACE_CLASS is ignored when implementing dual HID
#	endif
#endif

#ifdef USB_CFG_INTERFACE_SUBCLASS
#	if (USB_CFG_INTERFACE_SUBCLASS!=0)
#		warning USB_CFG_INTERFACE_SUBCLASS is ignored when implementing dual HID
#	endif
#endif

#ifdef USB_CFG_INTERFACE_PROTOCOL
#	if (USB_CFG_INTERFACE_PROTOCOL!=0)
#		warning USB_CFG_INTERFACE_PROTOCOL is ignored when implementing dual HID
#	endif
#endif


#ifndef USB_CFG_HID_NOKEYBOARD
typedef struct __keyboard_report_t keyboard_report_t;

struct __keyboard_report_t {
        uint8_t modifier;
        uint8_t reserved;
        uint8_t keycode[6];
} __attribute__((packed));
#endif

#ifndef USB_CFG_HID_NOMOUSE
typedef struct __mouse_report_t mouse_report_t;

#define mouse_report_buttonbit_left		0
#define mouse_report_buttonbit_right		1
#define mouse_report_buttonbit_middle		2

#define mouse_report_displacement_X		0
#define mouse_report_displacement_Y		1
#ifdef USB_CFG_HID_WHEELMOUSE
#	define mouse_report_displacement_wheel	2
#endif
struct __mouse_report_t {
        uint8_t buttons;
#ifdef USB_CFG_HID_WHEELMOUSE
         int8_t displacement[3]; // [0]=X, [1]=Y, [2]=wheel
#else
         int8_t displacement[2]; // [0]=X, [1]=Y
#endif
} __attribute__((packed));
#endif



#if (!(defined(USB_CFG_HID_NOKEYBOARD)))
#	define HIDKEYBOARD_LEDBIT_NUM_LOCK		0
#	define HIDKEYBOARD_LEDBIT_CAPS_LOCK		1
#	define HIDKEYBOARD_LEDBIT_SCROLL_LOCK		2
#	define HIDKEYBOARD_LEDBIT_COMPOSE		3
#	define HIDKEYBOARD_LEDBIT_KANA			4

#	define HIDKEYBOARD_BOOT_KEYUSAGE_NoEvent	0x00
#	define HIDKEYBOARD_BOOT_KEYUSAGE_ErrorRollOver	0x01
#	define HIDKEYBOARD_BOOT_KEYUSAGE_POSTFail	0x02
#	define HIDKEYBOARD_BOOT_KEYUSAGE_ErrorUndefined	0x03
#endif


HIDCOREPUBLIC PROGMEM const char usbDescriptorConfiguration[];

#ifndef USB_CFG_HID_NOKEYBOARD
HIDCOREPUBLIC keyboard_report_t	current_keyboard_report;
HIDCOREPUBLIC uint8_t		keyboard_report_dirty;
HIDCOREPUBLIC volatile uchar	current_LED_state; // keyboard LEDs

HIDCOREPUBLIC PROGMEM const char usbHidKeyboardReportDescriptor[];
HIDCOREPUBLIC void EVENT_CHANGE_LED_state (void);
HIDCOREPUBLIC void keyboard_report_fillkeys(keyboard_report_t *report, uint8_t usagecode);
HIDCOREPUBLIC void keyboard_report_setNoEvent(keyboard_report_t *report);
HIDCOREPUBLIC void keyboard_report_setErrorRollOver(keyboard_report_t *report);
HIDCOREPUBLIC void keyboard_report_clear(keyboard_report_t *report);
#endif

#ifndef USB_CFG_HID_NOMOUSE
HIDCOREPUBLIC mouse_report_t	current_mouse_report;
HIDCOREPUBLIC uint8_t		mouse_report_dirty;

HIDCOREPUBLIC PROGMEM const char usbHidMouseReportDescriptor[];
HIDCOREPUBLIC void mouse_report_clear(mouse_report_t *report);
#endif

HIDCOREPUBLIC usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq);

HIDCOREPUBLIC void hidInit(void);
HIDCOREPUBLIC void hidPoll(uint8_t *time_since_lastcall_in_four_ms_units);

HIDCOREPUBLIC usbMsgLen_t hidFunctionSetup(uchar data[8]);

#endif /* (USB_CFG_DESCR_PROPS_CONFIGURATION > 0) */

#endif

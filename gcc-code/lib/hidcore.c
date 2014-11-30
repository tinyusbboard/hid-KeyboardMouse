/*
 * hidcore.c
 * 
 * This file implements a comfortable API for HID
 * keyboard and Mouse devices using V-USB on
 * an Atmel ATmega microcontroller.
 * The device will be HID BOOT v1.1 conform, so it also
 * will work in BIOS and in legacy operating systems.
 * 
 * by Stephan Baerwolf (stephan@matrixstorm.com), Schwansee 2014
 * for http://matrixstorm.com/avr/tinyusbboard/
 * (please contact me at least before commercial use)
 */

#define __HIDCORE_C_7b57b2239b7d418b8332be68649a9120 1

#include "hidcore.h"

#include <string.h>

#if (USB_CFG_DESCR_PROPS_CONFIGURATION > 0)


#ifndef USB_CFG_HID_NOKEYBOARD
/* implement the HID BOOT conform keyboard
 * from page 59 and page 69 of:
 * Device Class Definition for Human Interface Devices (HID)
 * Firmware Specification--6/27/01
 * Version 1.11
 * http://www.usb.org/developers/hidpage/HID1_11.pdf
 */
keyboard_report_t	current_keyboard_report;
uint8_t			keyboard_report_dirty;
static uint8_t		keyboard_bootprotocol	= 1;	// 0 = boot protocol, 1 = report protocol
static uint8_t		keyboard_idleRate	= 125;	// repeat rate for keyboards in 4 ms units
static uint8_t		keyboard_idleTime	= 0;

volatile uchar		current_LED_state; // keyboard LEDs

PROGMEM const char usbHidKeyboardReportDescriptor[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,                    // USAGE (Keyboard)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0xe0,                    //   USAGE_MINIMUM (Keyboard LeftControl)(224)
    0x29, 0xe7,                    //   USAGE_MAXIMUM (Keyboard Right GUI)(231)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //   LOGICAL_MAXIMUM (1)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x95, 0x08,                    //   REPORT_COUNT (8)
    0x81, 0x02,                    //   INPUT (Data,Var,Abs) ; Modifier byte
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x81, 0x01,                    //   INPUT (Cnst,Var,Abs) ; Reserved byte
    0x95, 0x05,                    //   REPORT_COUNT (5)
    0x75, 0x01,                    //   REPORT_SIZE (1)
    0x05, 0x08,                    //   USAGE_PAGE (LEDs)
    0x19, 0x01,                    //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,                    //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,                    //   OUTPUT (Data,Var,Abs) ; LED report
    0x95, 0x01,                    //   REPORT_COUNT (1)
    0x75, 0x03,                    //   REPORT_SIZE (3)
    0x91, 0x01,                    //   OUTPUT (Cnst,Var,Abs) ; LED report padding
    0x95, 0x06,                    //   REPORT_COUNT (6)
    0x75, 0x08,                    //   REPORT_SIZE (8)
    0x15, 0x00,                    //   LOGICAL_MINIMUM (0)
    0x25, 0x65,                    //   LOGICAL_MAXIMUM (101)
    0x05, 0x07,                    //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0x00,                    //   USAGE_MINIMUM (Reserved (no event indicated))(0)
    0x29, 0x65,                    //   USAGE_MAXIMUM (Keyboard Application)(101)
    0x81, 0x00,                    //   INPUT (Data,Ary,Abs)
    0xc0                           // END_COLLECTION
};

void __attribute__((weak)) EVENT_CHANGE_LED_state (void) {
}

void keyboard_report_fillkeys(keyboard_report_t *report, uint8_t usagecode) {
  memset((void*)&report->keycode, usagecode, sizeof(report->keycode));
}
void keyboard_report_setNoEvent(keyboard_report_t *report) {
  keyboard_report_fillkeys(report, HIDKEYBOARD_BOOT_KEYUSAGE_NoEvent);
}
void keyboard_report_setErrorRollOver(keyboard_report_t *report) {
  keyboard_report_fillkeys(report, HIDKEYBOARD_BOOT_KEYUSAGE_ErrorRollOver);
}

void keyboard_report_clear(keyboard_report_t *report) {
  report->modifier = 0x00;
  report->reserved = 0x00;
  keyboard_report_setNoEvent(report);
}
#endif

#ifndef USB_CFG_HID_NOMOUSE
/* implement the HID BOOT conform mouse
 * from page 61 and page 71 of:
 * Device Class Definition for Human Interface Devices (HID)
 * Firmware Specification--6/27/01
 * Version 1.11
 * http://www.usb.org/developers/hidpage/HID1_11.pdf
 */
mouse_report_t		current_mouse_report;
uint8_t			mouse_report_dirty;
static uint8_t		mouse_bootprotocol	= 1;	// 0 = boot protocol, 1 = report protocol
static uint8_t		mouse_idleRate		= 0;	// repeat rate for mouses in 4 ms units
static uint8_t		mouse_idleTime		= 0;

PROGMEM const char usbHidMouseReportDescriptor[] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    // USAGE (Mouse)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x09, 0x01,                    //   USAGE (Pointer)
    0xa1, 0x00,                    //   COLLECTION (Physical)
    0x05, 0x09,                    //     USAGE_PAGE (Buttons)
    0x19, 0x01,                    //     USAGE_MINIMUM (01)
    0x29, 0x03,                    //     USAGE_MAXIMUM (03)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)   
    0x95, 0x03,                    //     REPORT_COUNT (3)
    0x75, 0x01,                    //     REPORT_SIZE (1)    
    0x81, 0x02,                    //     INPUT (Data,Var,Abs) ; 3 button bits
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x05,                    //     REPORT_SIZE (5)
    0x81, 0x01,                    //     INPUT (Cnst.) ; 5 bit padding
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //     USAGE_X
    0x09, 0x31,                    //     USAGE_Y
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (+127)   
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel) ; 2 position bytes (X & Y)
#ifdef USB_CFG_HID_WHEELMOUSE
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x38,                    //     USAGE (Wheel)
    0x15, 0x81,                    //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,                    //     LOGICAL_MAXIMUM (127)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)        - reset physical
    0x45, 0x00,                    //     PHYSICAL_MAXIMUM (0)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x81, 0x06,                    //     INPUT (Data,Var,Rel)
#endif
    0xc0,                          //   END_COLLECTION
    0xc0,                          // END_COLLECTION
};

void mouse_report_clear(mouse_report_t *report) {
  memset((void*)report, 0x0, sizeof(mouse_report_t));
}
#endif


PROGMEM const char usbDescriptorConfiguration[] = {    /* USB configuration descriptor */
    9,          /* sizeof(usbDescriptorConfiguration): length of descriptor in bytes */
    USBDESCR_CONFIG,    /* descriptor type */
    USB_CFG_DESCR_PROPS_CONFIGURATION, 0,
                /* total length of data returned (including inlined descriptors) */
    2,          /* number of interfaces in this configuration */
    1,          /* index of this configuration */
    0,          /* configuration name string index */
#if USB_CFG_IS_SELF_POWERED
    (1 << 7) | USBATTR_SELFPOWER,       /* attributes */
#else
    (1 << 7),                           /* attributes */
#endif
    USB_CFG_MAX_BUS_POWER/2,            /* max USB current in 2mA units */

#ifndef USB_CFG_HID_NOKEYBOARD
/* interface descriptor nr. 1 (keyboard) follows inline: */
    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    USBIFACE_INDEX_KEYBOARD, /* index of this interface */
    0,          /* alternate setting for this interface */
    1, /* endpoints excl 0: the keyboard endpoint */
    USBIFACE_CLASS_HID,
    USBIFACE_SUBCLASS_BOOT,
    USBIFACE_PROTOCOL_KEYBOARD,
    USBIFACE_NO_DESCRIPTOR,          /* string index for interface */
/* HID descriptor */
    9,          /* sizeof(usbDescrHID): length of descriptor in bytes */
    USBDESCR_HID,   /* descriptor type: HID */
    0x01, 0x01, /* BCD representation of HID version */
    0x00,       /* target country code */
    0x01,       /* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       /* descriptor type: report */
    sizeof(usbHidKeyboardReportDescriptor), 0,  /* total length of report descriptor */
/* the endpoint */
    7,          /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
    (char)0x81, /* IN endpoint number 1 */
    0x03,       /* attrib: Interrupt endpoint */
    8, 0,       /* maximum packet size */
    USB_CFG_INTR_POLL_INTERVAL, /* in ms */
#endif

#ifndef USB_CFG_HID_NOMOUSE    
/* interface descriptor nr. 2 (mouse) follows inline: */
    9,          /* sizeof(usbDescrInterface): length of descriptor in bytes */
    USBDESCR_INTERFACE, /* descriptor type */
    USBIFACE_INDEX_MOUSE, /* index of this interface */
    0,          /* alternate setting for this interface */
    1, /* endpoints excl 0: the keyboard endpoint */
    USBIFACE_CLASS_HID,
    USBIFACE_SUBCLASS_BOOT,
    USBIFACE_PROTOCOL_MOUSE,
    USBIFACE_NO_DESCRIPTOR,          /* string index for interface */
/* HID descriptor */
    9,          /* sizeof(usbDescrHID): length of descriptor in bytes */
    USBDESCR_HID,   /* descriptor type: HID */
    0x01, 0x01, /* BCD representation of HID version */
    0x00,       /* target country code */
    0x01,       /* number of HID Report (or other HID class) Descriptor infos to follow */
    0x22,       /* descriptor type: report */
    sizeof(usbHidMouseReportDescriptor), 0,  /* total length of report descriptor */
/* the endpoint */
    7,          /* sizeof(usbDescrEndpoint) */
    USBDESCR_ENDPOINT,  /* descriptor type = endpoint */
#ifndef USB_CFG_HID_NOKEYBOARD
    (char)(0x80 | USB_CFG_EP3_NUMBER), /* IN endpoint number 3 */
#else
    (char)0x81, /* IN endpoint number 1 */
#endif
    0x03,       /* attrib: Interrupt endpoint */
    8, 0,       /* maximum packet size */
    USB_CFG_INTR_POLL_INTERVAL, /* in ms */
#endif
};


usbMsgLen_t usbFunctionDescriptor(struct usbRequest *rq) {
  if (rq->wValue.bytes[1] == USBDESCR_HID) {
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
    /* ugly hardcoding data within usbDescriptorConfiguration */
    if (rq->wIndex.word == USBIFACE_INDEX_MOUSE) {
#endif
#ifndef USB_CFG_HID_NOMOUSE
#	ifndef USB_CFG_HID_NOKEYBOARD
      usbMsgPtr=(usbMsgPtr_t)&usbDescriptorConfiguration[43]; /* point to the HID mouse */
#	else
      usbMsgPtr=(usbMsgPtr_t)&usbDescriptorConfiguration[18]; /* point to the HID mouse */
#	endif
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
    } else {
#endif
#ifndef USB_CFG_HID_NOKEYBOARD
      usbMsgPtr=(usbMsgPtr_t)&usbDescriptorConfiguration[18]; /* point to the HID keyboard */
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
    }
#endif
  } else if (rq->wValue.bytes[1] == USBDESCR_HID_REPORT) {
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
    if (rq->wIndex.word == USBIFACE_INDEX_MOUSE) {
#endif
#ifndef USB_CFG_HID_NOMOUSE      
      usbMsgPtr=(usbMsgPtr_t)&usbHidMouseReportDescriptor[0];
      return sizeof(usbHidMouseReportDescriptor);
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
    } else {
#endif
#ifndef USB_CFG_HID_NOKEYBOARD
      usbMsgPtr=(usbMsgPtr_t)&usbHidKeyboardReportDescriptor[0];
      return sizeof(usbHidKeyboardReportDescriptor);
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
    }
#endif
  }

  return 0;
}


void hidInit(void) {
  // keyboard
#ifndef USB_CFG_HID_NOKEYBOARD
  keyboard_report_clear(&current_keyboard_report);
  keyboard_report_dirty	= 0;
  keyboard_bootprotocol	= 1;

  keyboard_idleRate	= 125;
  keyboard_idleTime	= 0;

  // LEDs
  current_LED_state	= 0xff;
#endif
  
  // mouse
#ifndef USB_CFG_HID_NOMOUSE 
  mouse_report_clear(&current_mouse_report);
  mouse_report_dirty	= 0;
  mouse_bootprotocol	= 1;

  mouse_idleRate	= 0;
  mouse_idleTime	= 0;
#endif  
}


void hidPoll(uint8_t *time_since_lastcall_in_four_ms_units) {
  uint8_t _time = (*time_since_lastcall_in_four_ms_units);

  usbPoll();

#ifndef USB_CFG_HID_NOKEYBOARD
  // is it time to file a report: keyboard
  if (keyboard_idleRate > 0) {
    keyboard_idleTime	+= _time;
    if (keyboard_idleTime >= keyboard_idleRate) {
      keyboard_idleTime		 = 0;
      keyboard_report_dirty	|= 0x80;
    }
  }
#endif

#ifndef USB_CFG_HID_NOMOUSE
  // is it time to file a report: mouse
  if (mouse_idleRate > 0) {
    mouse_idleTime	+= _time;
    if (mouse_idleTime >= mouse_idleRate) {
      mouse_idleTime	 	 = 0;
      mouse_report_dirty	|= 0x80;
    }
  }
#endif

#ifndef USB_CFG_HID_NOKEYBOARD
  if (keyboard_report_dirty)
    if (usbInterruptIsReady()) {
      usbSetInterrupt((void *)&current_keyboard_report, sizeof(current_keyboard_report));
      keyboard_report_dirty = 0;
    }
#endif

#ifndef USB_CFG_HID_NOMOUSE
  if (mouse_report_dirty)
#ifndef USB_CFG_HID_NOKEYBOARD
    if (usbInterruptIsReady3()) {
      usbSetInterrupt3((void *)&current_mouse_report, sizeof(current_mouse_report));
#else
    if (usbInterruptIsReady()) {
      usbSetInterrupt((void *)&current_mouse_report, sizeof(current_mouse_report));
#endif
      mouse_report_dirty = 0;
    }
#endif

  usbPoll();
}


usbMsgLen_t __attribute__((weak)) hidFunctionSetup(uchar data[8]) {
  return 0;
}

USB_PUBLIC usbMsgLen_t usbFunctionSetup(uchar data[8]) {
    usbRequest_t *rq = (void *)data;

/* from page 50 of:
 * Device Class Definition for Human Interface Devices (HID)
 * Firmware Specification--6/27/01
 * Version 1.11
 * http://www.usb.org/developers/hidpage/HID1_11.pdf
 */
    if((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
      switch(rq->bRequest) {
	case USBRQ_HID_GET_REPORT: {
	  // send "no keys pressed" and "no mouse event" if asked here
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  usbWord_t index = rq->wIndex;
#endif
	  usbMsgPtr = (usbMsgPtr_t)&data[0]; // we only have this one
	  memset((void*)usbMsgPtr, 0x00, sizeof(data));
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  if (index.word==USBIFACE_INDEX_MOUSE) {
#endif
#ifndef USB_CFG_HID_NOMOUSE
	    // mouse
	    return sizeof(current_mouse_report);
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  } else {
#endif
#ifndef USB_CFG_HID_NOKEYBOARD
	    // keyboard
	    return sizeof(current_keyboard_report);
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  }
#endif
	}
	case USBRQ_HID_SET_REPORT: {
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  if (rq->wIndex.word==USBIFACE_INDEX_MOUSE) {
#endif
#ifndef USB_CFG_HID_NOMOUSE
	    // mouse
	    return 0;
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  } else {
#endif
#ifndef USB_CFG_HID_NOKEYBOARD
	    // keyboard
	    // if wLength == 1, should be LED state
	    return (rq->wLength.word == 1) ? USB_NO_MSG : 0;
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  }
#endif
	}
	
	case USBRQ_HID_GET_IDLE: {
	  // send idle rate to PC as required by spec
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  usbMsgPtr = (rq->wIndex.word==USBIFACE_INDEX_MOUSE)?(usbMsgPtr_t)&mouse_idleRate:(usbMsgPtr_t)&keyboard_idleRate;
#else
#ifndef USB_CFG_HID_NOMOUSE
	  usbMsgPtr = (usbMsgPtr_t)&mouse_idleRate;
#endif
#ifndef USB_CFG_HID_NOKEYBOARD
	  usbMsgPtr = (usbMsgPtr_t)&keyboard_idleRate;
#endif
#endif
	  return 1;
	}
	case USBRQ_HID_SET_IDLE: {
	  // save idle rate as required by spec
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  if (rq->wIndex.word==USBIFACE_INDEX_MOUSE) {
#endif
#ifndef USB_CFG_HID_NOMOUSE
	    mouse_idleRate = rq->wValue.bytes[1];
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  } else {
#endif
#ifndef USB_CFG_HID_NOKEYBOARD
	    keyboard_idleRate = rq->wValue.bytes[1];
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  }
#endif
	  return 0;
	}
	
	case USBRQ_HID_GET_PROTOCOL: {
	  // send protocol to PC as required by spec
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  usbMsgPtr = (rq->wIndex.word==USBIFACE_INDEX_MOUSE)?(usbMsgPtr_t)&mouse_bootprotocol:(usbMsgPtr_t)&keyboard_bootprotocol;
#else
#ifndef USB_CFG_HID_NOMOUSE
	  usbMsgPtr = (usbMsgPtr_t)&mouse_bootprotocol;
#endif
#ifndef USB_CFG_HID_NOKEYBOARD
	  usbMsgPtr = (usbMsgPtr_t)&keyboard_bootprotocol;
#endif
#endif
	  return 1;
	}
	case USBRQ_HID_SET_PROTOCOL: {
	  // save idle rate as required by spec
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  if (rq->wIndex.word==USBIFACE_INDEX_MOUSE) {
#endif
#ifndef USB_CFG_HID_NOMOUSE
	    mouse_bootprotocol = rq->wValue.bytes[0];
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  } else {
#endif
#ifndef USB_CFG_HID_NOKEYBOARD
	    keyboard_bootprotocol = rq->wValue.bytes[0];
#endif
#if ((!defined(USB_CFG_HID_NOMOUSE)) && (!defined(USB_CFG_HID_NOKEYBOARD)))
	  }
#endif
	  return 0;
	}
      }
    }
    
    return hidFunctionSetup(data); // by default don't return any data
}

#ifndef USB_CFG_HID_NOKEYBOARD
USB_PUBLIC usbMsgLen_t usbFunctionWrite(uint8_t * data, uchar len) {
  if (data[0] != current_LED_state) {
    current_LED_state = data[0];
    EVENT_CHANGE_LED_state(); //state change
  }
  return 1; // Data read, not expecting more
}
#endif


#endif /* (USB_CFG_DESCR_PROPS_CONFIGURATION > 0) */
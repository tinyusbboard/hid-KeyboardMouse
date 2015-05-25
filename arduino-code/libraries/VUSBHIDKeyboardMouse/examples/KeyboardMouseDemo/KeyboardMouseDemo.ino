#include "VUSBHIDKeyboardMouse.h"

#define LED_KLICK	3
#define LED_BLINK	5
#define BUTTON_PIN	6

#define TYPEDELAY_4MS	20
#define WIGGLEDELAY_4MS	10

/* 
 * in order to work without timer, the following value configures timing 
 * please calibrate it, to see the "LED_BLINK" blink 
 * with about 8sec (4sec on and 4sec off)..
 */
#define TIMECTRLTUNE	350


const unsigned char message[] PROGMEM  = {
"tinyUSBboard\n"
"http://matrixstorm.com/avr/tinyusbboard/\n"
"\n"
"\n"
"In case you encouter problems, have any questions or in case\n"
"you just have remarks, please do not hesitate to contact:\n"
"Stephan Baerwolf  - stephan@matrixstorm.com\n"
"matrixstorm@gmx.de, or stephan.baerwolf@tu-ilmenau.de\n"
"\n"
"PGP-KeyID\t: 0xFB9B12DB\n"
"Fingerprint\t: 2252 B41E 12DF 41AD 476F 7F36 084A 0911 FB9B 12DB\n"
"\n"
"\n"
"\n"
};

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
  randomSeed(analogRead(0));

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(BUTTON_PIN, HIGH);

  pinMode(LED_KLICK, OUTPUT);

  /* start with LED switched on */
  pinMode(LED_BLINK, OUTPUT);
  digitalWrite(LED_BLINK, 1);
}


static byte    typeDelayCounter		= 0;
static byte    wiggleDelayCounter	= 0;
static size_t  charPosition   		= sizeof(message); /* message symbol */

static uint16_t toSecondsCounter	= 0;
static uint16_t timeCalibrationCounter	= 0;
void loop() {
  timeCalibrationCounter++;

  if (timeCalibrationCounter<TIMECTRLTUNE) {
    VUSBHIDKeyboardMouse.update(0);
  } else {
    /* every 4ms */
    timeCalibrationCounter=0;
    
    /* keyboard */
    typeDelayCounter++;
    if (typeDelayCounter >= TYPEDELAY_4MS) {
      typeDelayCounter=0;
      if (!digitalRead(BUTTON_PIN)) {
	digitalWrite(LED_KLICK, 1);
	charPosition++;

	//type a key
	if (charPosition>=sizeof(message)) charPosition=0;
	UsbKeyboard.write(pgm_read_byte_near(message + charPosition));
      } else digitalWrite(LED_KLICK, 0);
    }

    /* mouse */
    wiggleDelayCounter++;
    if (wiggleDelayCounter >= WIGGLEDELAY_4MS) {
      // wiggle the mouse
      wiggleDelayCounter=0;
      UsbMouse.move(random(-8,8), random(-8,8));
    }

    VUSBHIDKeyboardMouse.update(1);
    toSecondsCounter++;
  }


  /* the calibrated frequency to measure */
  if (toSecondsCounter >= 1000) {
    toSecondsCounter=0;
    digitalWrite(LED_BLINK, !digitalRead(LED_BLINK));
  }
}

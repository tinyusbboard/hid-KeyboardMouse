hid-KeyboardMouse
=================

This is a library for implementing both a HID (boot) compliant Keyboard and
Mouse based on V-USB at the same time.
All the USB stuff is primarily handled internally and abstracted.

It is designed (but not limited to) tinyUSBboard and can be used in
(avr-gc)C and Arduino. (The latter one is based on the C-Code).
While the C integration is more native (and fast), the Arduino-
library is called VUSBHIDKeyboardMouse.

Additional features like mousewheel support can be activated for C,
via external defines in the Makefile. 

by Stephan Baerwolf (matrixstorm@gmx.de), Schwansee 2014

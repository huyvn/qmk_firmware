ps2avrGB keyboard firmware
==========================

This is a port of the QMK firmware for boards that are based on the
ps2avrGB firmware, like the [ps2avrGB
keyboard](https://www.keyclack.com/product/gb-ps2avrgb/) or the ones sold
by [Winkeyless](http://winkeyless.kr/product/ps2avrgb-parts/).

Note that this is a complete replacement for the firmware, so you won't be
using Bootmapper Client to change any keyboard settings, since not all the
USB report options are supported.

This is an example based on the VE.A clone split keyboard on Taobao for making
it compatible with QMK

## Installing

Follow [Getting Started](https://github.com/qmk/qmk_firmware/blob/master/docs/newbs_getting_started.md)
to setup your environment. That should setup your `AVR` toolchain and `bootloadHID`. 

Refer to [here](https://github.com/qmk/qmk_firmware/blob/master/docs/flashing_bootloadhid.md)
if have trouble with setting up `bootloadHID`.

## Compiling firmware

Run this command from the `qmk_firmware` directory:

```
$ make ps2avrGB/split75
```

## Flashing firmware

Boot the board while holding down `L_Ctrl` to keep it in the bootloader.
Then run this command to flash the firmware directly with `bootloadHID`. You may need `sudo`.

```
$ bootloadHID -r ps2avrGB_<keymap>.hex
```

## Setting the board to bootloader mode

You can enter the bootloader mode manually by plugging the keyboard in while 
holding the bootloader key. If you did this correctly the LEDs will blink and 
you'll be able to flash your firmware.

The bootloader key is the top left key of your matrix. For a standard board
from winkeyless.kr this is `L_Ctrl`, but for some custom boards running ps2avrGB
this might be different. For the Pearl it's `Esc`, for a b.fake it is `q`. If 
you're unsure you should be able to see the key in the bootmapper client.


## Troubleshooting

From the original guy's experience, it's really hard to brick these boards. 
But these tricks have been useful when it got stuck in a weird scenario.

1. Try plugging the board in while holding the bootloader key. This will force 
   it to boot only the bootloader without loading the firmware. Once this is
   done, just reflash the board with the original firmware.
2. Sometimes USB hubs can act weird, so try connecting the board directly
   to your computer or plugging/unplugging the USB hub.

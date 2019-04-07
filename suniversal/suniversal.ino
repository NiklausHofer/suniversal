/*
    suniversal - USB Adapter for SUN Type 5 Keyboards
    Copyright (c) 2017, Alexander Vollschwitz

    based on SunType5 ArduinoAdapter by Ben Rockwood
    https://github.com/benr/SunType5_ArduinoAdapter

    developed on: Arduino Pro Micro

    This file is part of suniversal.

    suniversal is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    suniversal is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with suniversal. If not, see <http://www.gnu.org/licenses/>.
*/

#include <SoftwareSerial.h>

#include "config.h"
#include "keyboard.h"
#include "mouse.h"

// Arduino pins
#define PIN_RX 10
#define PIN_TX  9

// SUN power key
#define POWER            0x30

// SUN toggle keys
#define NUM_LOCK         0x62
#define NUM_LOCK_MASK    0x01
#define COMPOSE          0x43
#define COMPOSE_MASK     0x02
#define SCROLL_LOCK      0x17
#define SCROLL_LOCK_MASK 0x04
#define CAPS_LOCK        0x77
#define CAPS_LOCK_MASK   0x08
#define ALL_LEDS         0x0F

// SUN keyboard command codes
#define CMD_RESET        0x01
#define CMD_BELL_ON      0x02
#define CMD_BELL_OFF     0x03
#define CMD_CLICK_ON     0x0A
#define CMD_CLICK_OFF    0x0B
#define CMD_LED          0x0E
#define CMD_LAYOUT       0x0F

// SUN keyboard report codes
#define KBD_IDLE         0x7F
#define KBD_LAYOUT_RESP  0xFE
#define KBD_RESET_RESP   0xFF

// key break bit is bit 7
#define BREAK_BIT        0x80

// LED command sequence
uint8_t cmdLED[2] = {CMD_LED, 0x00};

// for communication with the SUN keyboard
SoftwareSerial sun(PIN_RX, PIN_TX, false);

// SNAFU flag
bool keyboardBroken = false;

// for turning off Compose key after next two key strokes
uint8_t count_to_compose_off = 0;

//
void setup() {

    if (DEBUG) {
        Serial.begin(1200, SERIAL_8N1);
    }

    if (USE_MOUSE) {
        // mouse gets hooked to the H/W serial, which on the Pro Micro is
        // Serial1. IMPORTANT: Just like the keyboard, the mouse also uses
        // inverted serial signal, so you need an inverter in the line between
        // the mouse and RX of the Arduino, e.g. a transistor and two resistors
        // (Tx->15kOhm->B, C->Rx, 5V->10kOhm->Rx, E->GND).
        Serial1.begin(1200, SERIAL_8N2);
    }

    sun.begin(10500);
    //resetKeyboard();
}

/*
    From keyboard documentation: After receiving the reset command, the
    keyboard executes the self test routine. The keyboard responds with:
    0FFH, 004H, 007FH if the self test passes and no keys are down. The
    code 07FH is replaced by the make code if a key is down. The keyboard
    sends 07EH, 001H if the self test fails.
 */
void resetKeyboard() {

    DPRINTLN("resetting keyboard");

    // read anything that may have come in after power-up
    clearFromBuffer(0);
    sun.write(CMD_RESET);

    // if first byte is keyboard reset response, we consider that success
    if (waitAndRead() == KBD_RESET_RESP) {

        DPRINTLN("keyboard ok");
        // wait for & discard the remainder of the response
        waitForResponse(2);
        clearFromBuffer(2);

        if (USE_MACROS) {
            keyboardConverter.setLayout(getLayout());
        }

        sun.write(cmdLED, 2); // reset LEDs

        if (STARTUP_GREETING) {
            flashLEDs(CAPS_LOCK_MASK);
            flashLEDs(SCROLL_LOCK_MASK);
            flashLEDs(NUM_LOCK_MASK);
            flashLEDs(COMPOSE_MASK);
            flashLEDs(ALL_LEDS);
            beep(75);
            beep(75);
        }

    } else {
        DPRINTLN("keyboard broken");
        keyboardBroken = true;
        for (uint8_t i = 0; i < 8; i++) {
            beep(125);
        }
    }
}

/*
    Depending on the keyboard layout in use, we may have to adjust some of the
    macros. For example, if we want to send Ctrl-Z to the host for an Undo, we
    send the scan codes for Control and Z. If the keyboard layout however is for
    example German, the Z and Y keys will be swapped, and the host will interpret
    the scan code for Z actually as a Y, and we end up with Ctrl-Y (Redo).

    Now there's no way of knowing what layout is active on the host, but in most
    cases, it will be the same as is set in the keyboard itself. So we get the
    layout from the keyboard here and pass it to the converter to make the
    necessary adjustments (see resetKeyboard). If this is not not desired, you
    can force a particular layout with the FORCE_LAYOUT setting in config.h.
 */
uint8_t getLayout() {

    if (FORCE_LAYOUT != GET_FROM_KEYBOARD) {
        DPRINTLN("using forced layout: " + String(FORCE_LAYOUT));
        return FORCE_LAYOUT;
    }

    sun.write(CMD_LAYOUT);

    if (waitAndRead() != KBD_LAYOUT_RESP) {
        DPRINTLN("could not determine keyboard layout, defaulting to US");
        return UNITED_STATES;
    }

    // The Type 5 has 8 DIP switch, while on the Type 5c I have, there are only
    // 5, corresponding to bits 4 through 8 of the layout code. Bit 3 is always
    // 1, and bits 2 and 1 are 0 per documentation anyway. Therefore masking out
    // bits 1,2 and 3. Works with both keyboards.
    uint8_t l = waitAndRead() & 0x1F;

    switch (l) {
        case UNITED_STATES:
        case FRENCH_BELGIUM:
        case CANADA_FRENCH:
        case DENMARK:
        case GERMANY:
        case ITALY:
        case NETHERLANDS:
        case NORWAY:
        case PORTUGAL:
        case SPAIN_LATIN_AMERICA:
        case SWEDEN_FINLAND:
        case SWISS_FRENCH:
        case SWISS_GERMAN:
        case UNITED_KINGDOM:
            DPRINTLN("keyboard layout: " + String(l));
            return l;
    }

    DPRINTLN("invalid keyboard layout: " + String(l) + ", defaulting to US");
    return UNITED_STATES;
}

/*
    On the Pro Micro, and a few other boards, we need to use serialEventRun
    instead of serialEvent. See for example:
        http://forum.arduino.cc/index.php?topic=135011.0
 */
void serialEventRun() {
    while (Serial1.available()) {
        mouseConverter.update(Serial1.read());
    }
}

void loop() {

    if (keyboardBroken) {
        flashLEDs(ALL_LEDS);
        return;
    }

    updateLEDs();

    while (sun.available() > 0) {

        int key = sun.read();

        switch (key) {
            case POWER:
                if (DEBUG) {
                    // in debug mode, power key resets keyboard
                    resetKeyboard();
                    return;
                } else {
                    usbKeyboard.wakeupHost();
                }
                break;
            case COMPOSE:
                if (COMPOSE_MODE) {
                    toggleLEDs(COMPOSE_MASK);
                    count_to_compose_off =
                        (cmdLED[1] & COMPOSE_MASK) == 0 ? 0 : 3;
                }
                break;
            case -1: // shouldn't really happen
                continue;
        }

        // check on every key release whether Compose needs to be switched off
        if ((key & BREAK_BIT) != 0) {
            switch (count_to_compose_off) {
                case 0:
                    break;
                case 1:
                    toggleLEDs(COMPOSE_MASK);
                default:
                    count_to_compose_off--;
                    break;
            }
        }

        handleKey(key);
    }
}

void handleKey(uint8_t key) {
    if (key == KBD_IDLE) {
        DPRINTLN("suniversal: all released");
        keyboardConverter.releaseAll();
    } else {
        bool pressed = (key & BREAK_BIT) == 0;
        key &= (~BREAK_BIT); // mask out break bit
        DPRINT("suniversal: " + String(key, HEX));
        DPRINTLN(pressed ? " down" : " up");
        keyboardConverter.handleKey(key, pressed);
    }
    DPRINTLN();
}

void updateLEDs() {
    uint8_t leds = usbKeyboard.getLeds();
    leds = ((leds & USB_LED_CAPS_LOCK) << 2) |
           ((leds & USB_LED_COMPOSE) >> 2) |
            (leds & (USB_LED_NUM_LOCK | USB_LED_SCROLL_LOCK));
    if (cmdLED[1] != leds) {
        DPRINTLN("suniversal: LED state changed: " + String(cmdLED[1], HEX) +
            " --> " + String(leds, HEX));
        cmdLED[1] = leds;
        sun.write(cmdLED, 2);
    }
}

void toggleLEDs(byte mask) {
    cmdLED[1] ^= mask;
    sun.write(cmdLED, 2);
}

void flashLEDs(byte mask) {
    toggleLEDs(mask);
    delay(200);
    toggleLEDs(mask);
    delay(200);
}

void beep(unsigned long duration) {
    sun.write(CMD_BELL_ON);
    delay(duration);
    sun.write(CMD_BELL_OFF);
    delay(duration);
}

/*
    Clear `count` number of bytes from serial input buffer. Does not block if
    there are less bytes present. If `count` is 0, clears all bytes present in
    buffer. Returns number of bytes removed from buffer.
 */
uint8_t clearFromBuffer(int8_t count) {
    for (uint8_t i = 0; sun.available() > 0; i++) {
        if (count > 0 && i == count) {
            return i;
        }
        sun.read();
    }
}

/*
    Waits until at least `expected` number of bytes are available in the serial
    input buffer.
 */
void waitForResponse(uint8_t expected) {
    while (sun.available() < expected) {
        delay(50);
    }
}

/*
    Read a single byte form the serial connection. Wait if no byte is available.
 */
uint8_t waitAndRead() {
    waitForResponse(1);
    return sun.read();
}

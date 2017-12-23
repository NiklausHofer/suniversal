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

#include "suniversal.h"
#include "converter.h"

// Arduino pins
#define PIN_RX 10
#define PIN_TX  9

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

// LED command sequence
uint8_t cmdLED[2] = {CMD_LED, 0x00};

// for communication with the SUN keyboard
SoftwareSerial sun(PIN_RX, PIN_TX, true);

// SNAFU flag
bool keyboardBroken = false;

void setup() {
    #ifdef SERIAL_DEBUG
    Serial.begin(1200);
    #endif
    sun.begin(1200);
    resetKeyboard();
}

/*
    From keyboard documentation: After receiving the reset command, the
    keyboard executes the self test routine. The keyboard responds with:
    0FFH, 004H, 007FH if the self test passes and no keys are down. The
    code 07FH is replaced by the make code if a key is down. The keyboard
    sends 07EH, 001H if the self test fails.
 */
void resetKeyboard() {

    // read anything that may have come in after power-up
    clearInputBuffer();
    sun.write(CMD_RESET);

    // wait for response
    while (sun.available() < 1) {
        delay(50);
    }

    // if first byte is keyboard reset response, we consider that success
    if (sun.read() == KBD_RESET_RESP) {
        clearInputBuffer(); // read extraneous bytes
        sun.write(cmdLED, 2); // reset LEDs
        flashLEDs(CAPS_LOCK_MASK);
        flashLEDs(SCROLL_LOCK_MASK);
        flashLEDs(NUM_LOCK_MASK);
        flashLEDs(COMPOSE_MASK);
        flashLEDs(ALL_LEDS);
        beep(75);
        beep(75);
    } else {
        keyboardBroken = true;
        for (uint8_t i = 0; i < 8; i++) {
            beep(125);
        }
    }
}

void loop() {

    if (keyboardBroken) {
        flashLEDs(ALL_LEDS);
        return;
    }

    while (sun.available() > 0) {
        int key = sun.read();
        switch (key) {
            case CAPS_LOCK:
                toggleLEDs(CAPS_LOCK_MASK);
                break;
            case SCROLL_LOCK:
                toggleLEDs(SCROLL_LOCK_MASK);
                break;
            case NUM_LOCK:
                toggleLEDs(NUM_LOCK_MASK);
                break;
            case COMPOSE:
                toggleLEDs(COMPOSE_MASK);
                break;
            case -1: // shouldn't really happen
                continue;
        }
        handleKey(key);
    }
}

void handleKey(uint8_t key) {
    if (key == KBD_IDLE) {
        DPRINTLN("suniversal: all released");
        converter.releaseAll();
    } else {
        bool pressed = (key & 0x80) == 0; // break code if bit 7 set
        key &= 0x7F; // mask break bit
        DPRINT("suniversal: " + String(key, HEX));
        DPRINTLN(pressed ? " down" : " up");
        converter.handleKey(key, pressed);
    }
    DPRINTLN();
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

void clearInputBuffer() {
    while (sun.available() > 0) {
        sun.read();
    }
}

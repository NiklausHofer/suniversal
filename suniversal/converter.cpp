/*
    scan code converter
    Copyright (c) 2017, Alexander Vollschwitz

    based on Arduino Keyboard library, Copyright (c) 2015, Arduino LLC
    https://github.com/arduino-libraries/Keyboard

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

#include "suniversal.h"
#include "converter.h"
#include "sun_to_usb.h"
#include "macros.h"

#define array_len( x )  ( sizeof( x ) / sizeof( *x ) )

#if defined(_USING_HID)

static const uint8_t hidReportDescriptor[] PROGMEM = {
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)  // 47
    0x09, 0x06, // USAGE (Keyboard)
    0xa1, 0x01, // COLLECTION (Application)
    0x85, 0x02, //   REPORT_ID (2)
    0x05, 0x07, //   USAGE_PAGE (Keyboard)

    0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x01, //   LOGICAL_MAXIMUM (1)
    0x75, 0x01, //   REPORT_SIZE (1)

    0x95, 0x08, //   REPORT_COUNT (8)
    0x81, 0x02, //   INPUT (Data,Var,Abs)
    0x95, 0x01, //   REPORT_COUNT (1)
    0x75, 0x08, //   REPORT_SIZE (8)
    0x81, 0x03, //   INPUT (Cnt, Var, Abs)

    0x95, 0x06, //   REPORT_COUNT (6)
    0x75, 0x08, //   REPORT_SIZE (8)
    0x15, 0x00, //   LOGICAL_MINIMUM (0)
    0x25, 0x81, //   LOGICAL_MAXIMUM (101)
    0x05, 0x07, //   USAGE_PAGE (Keyboard)

    0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0x81, //   USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, //   INPUT (Data, Ary, Abs)
    0xc0,       // END_COLLECTION
};

MacroTable macros;

/*
    KEY REPORT
 */
KeyReport::KeyReport() {
    releaseAll();
}

/*
    Sets or clears bit in modifiers byte according to m. Returns
    true if m is a modifier, i.e. non-zero, false otherwise.
 */
bool KeyReport::handleModifier(uint8_t m, bool pressed) {

    DPRINT("KeyReport.handleModifier: " +
        String(m, HEX) + ", " + String(pressed));

    if (m == 0) {
        DPRINTLN(" --> not a modifier");
        return false;
    }

    if (pressed) {
        data.modifiers |= m;
    } else {
        data.modifiers &= ~m;
    }

    DPRINTLN();
    return true;
}

/*
    Handle key k, adding or removing it to the report, according to
    pressed. Returns true if k was handled, false otherwise.
 */
bool KeyReport::handleKey(uint8_t k, bool pressed) {
    if (k == 0) {
        return false;
    }
    if (pressed) {
        return addKey(k);
    } else {
        return removeKey(k);
    }
}

/*
    Add k to the key report only if it's not already present
    and if there is an empty slot. Returns true if k was added,
    false otherwise.
 */
bool KeyReport::addKey(uint8_t k) {

    DPRINT("KeyReport.addKey: " + String(k, HEX));

    uint8_t value;
    int8_t slot = -1, i;

    for (i = 0; i < array_len(data.keys); i++) {
        value = data.keys[i];
        if (slot == -1 && value == 0) {
            slot = i;
        } else if (value == k) {
            break;
        }
    }

    if (i == array_len(data.keys) && slot > -1) {
        DPRINTLN(" --> adding at slot " + String(slot));
        data.keys[slot] = k;
        return true;
    }

    DPRINTLN(" --> not adding, slot=" + String(slot) + ", i=" + String(i));
    return false;
}

/*
    Clear k if it exists. Returns true if k was removed, false otherwise.
 */
bool KeyReport::removeKey(uint8_t k) {

    DPRINT("KeyReport.removeKey: " + String(k, HEX));

    for (uint8_t i = 0; i < array_len(data.keys); i++) {
        if (data.keys[i] == k) {
            DPRINTLN(" --> removing from slot " + String(i));
            data.keys[i] = 0;
            return true;
        }
    }

    DPRINTLN(" --> not found");
    return false;
}

/*
    Clear all keys and reset modifier bits.
 */
KeyReport::releaseAll() {
    memset(data.keys, 0, sizeof(data.keys));
    data.modifiers = 0;
}

KeyReport::send() {
    HID().SendReport(2, &data, sizeof(ReportData));
    DPRINT("KeyReport.send: modifiers=" +
        String(data.modifiers, HEX) + ", keys=[");
    if (DEBUG) {
        for (uint8_t i = 0; i < array_len(data.keys); i++) {
            DPRINT(" " + String(data.keys[i], HEX));
        }
    }
    DPRINTLN(" ]");
}

//
// CONVERTER
//
Converter::Converter() {
    static HIDSubDescriptor node(hidReportDescriptor, sizeof(hidReportDescriptor));
    HID().AppendDescriptor(&node);
}

Converter::setLayout(uint8_t layout) {
    macros.adjustToLayout(layout);
}

/*
    If pressed, add the specified key to the key report and send the report.
    Because of the way USB HID works, the host acts as if the key remains pressed
    until we clear the report and resend.

    If not pressed, take the specified key out of the key report and send the
    report. This tells the OS the key is no longer pressed and that it shouldn't
    be repeated any more.
 */
Converter::handleKey(uint8_t sunKey, bool pressed) {
    uint16_t usbKey = sun2usb[sunKey];
    DPRINTLN("Converter.handleKey: " +
        String(sunKey, HEX) + " --> " + String(usbKey, HEX));
    if (usbKey > 0) {
        if (USE_MACROS && handleMacro(usbKey, pressed)) {
            return;
        }
        // modifiers are in high byte, non-modifiers in low byte
        if (keyReport.handleModifier(usbKey >> 8, pressed) ||
            keyReport.handleKey(0xFF & usbKey, pressed)) {
            keyReport.send();
        }
    }
}

/*

 */
bool Converter::handleMacro(uint16_t k, bool pressed) {

    DPRINT("Converter.handleMacro: " +
        String(k, HEX) + ", " + String(pressed));
    if ((k & 0xFF00) != 0xFF00) {
        DPRINTLN(" --> not a macro");
        return false;
    }
    DPRINTLN();

    uint16_t* macro = macros.get(0xFF & k);
    for (uint8_t i = 0; macro[i] > 0; i++) {
        uint16_t m = macro[i];
        if (keyReport.handleModifier(m >> 8, pressed) ||
            keyReport.handleKey(0xFF & m, pressed)) {
            keyReport.send();
        }
    }

    return true;
}

/*
    Clear report and send it.
 */
Converter::releaseAll() {
    keyReport.releaseAll();
    keyReport.send();
}

Converter converter;

#endif

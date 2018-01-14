/*
    keyboard scan code converter
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

#ifndef KEYBOARD_CONVERTER_h
#define KEYBOARD_CONVERTER_h

#include <Arduino.h>
#include "HID-Project.h"

/*
    We use BootKeyboard from NicoHood HID project to do the USB heavy
    lifting. Everything keyboard-related however remains in suniversal.
 */
class USBAdapter : public BootKeyboard_ {

public:
    USBAdapter();
    int send(const void* data, int len);
};

extern USBAdapter usbAdapter;

/*
    key report data, up to 6 keys and modifiers at once
 */
typedef struct ReportData {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[6];
};

/*
    handles keys & modifiers
 */
class KeyReport {

private:
    ReportData data;
    bool addKey(uint8_t k);
    bool removeKey(uint8_t k);

public:
    KeyReport();
    bool handleModifier(uint8_t k, bool pressed);
    bool handleKey(uint8_t k, bool pressed);
    releaseAll();
    send();
};

/*
    the scan code converter
 */
class KeyboardConverter {

private:
    KeyReport keyReport;
    bool handleMacro(uint16_t k, bool pressed);

public:
    KeyboardConverter();
    setLayout(uint8_t layout);
    handleKey(uint8_t k, bool pressed);
    releaseAll();
};

extern KeyboardConverter keyboardConverter;

#endif

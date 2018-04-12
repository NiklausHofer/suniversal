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

#include "usb_keyboard.h"

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

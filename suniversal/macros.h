/*
    macros - macro definitions for special keys
    Copyright (c) 2017, Alexander Vollschwitz

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

#ifndef MACROS_h
#define MACROS_h

#include <stdint.h>
#include "usb_codes.h"

#define CONTROL (USB_MOD_LCTRL << 8)
#define ALT     (USB_MOD_LALT  << 8)

// macro index numbers
enum MACROS {
    MACRO_AGAIN = 0,
    MACRO_UNDO,
    MACRO_COPY,
    MACRO_PASTE,
    MACRO_CUT,
    MACRO_STOP,
    MACRO_PROPS,
    MACRO_FRONT,
    MACRO_OPEN,
    MACRO_FIND,
    MACRO_HELP,
    END_OF_MACROS
};

class MacroTable {

private:
    uint16_t* table[END_OF_MACROS];

public:
    MacroTable();
    adjustToLayout(uint8_t layout);
    uint16_t* get(uint8_t ix);
};

#endif

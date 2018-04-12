/*
    USB mouse
    Copyright (c) 2018, Alexander Vollschwitz

    based on original Arduino Mouse library code, Copyright (c) 2015, Arduino LLC

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

#ifndef USB_MOUSE_h
#define USB_MOUSE_h

#include <HID.h>

// ---------------------------------------------------------------------------

#if !defined(_USING_HID)
#warning "Using legacy HID core (non pluggable)"
#else

// ---------------------------------------------------------------------------

#define MOUSE_LEFT    1
#define MOUSE_RIGHT   2
#define MOUSE_MIDDLE  4
#define MOUSE_ALL (MOUSE_LEFT | MOUSE_RIGHT | MOUSE_MIDDLE)

/*

 */
class USBMouse {

private:
    uint8_t reportData[5];
    uint8_t buttons;
    setButtons(uint8_t b);
    send(uint8_t x, uint8_t y, uint8_t v, uint8_t h);

public:
    USBMouse();
    move(signed char x, signed char y);
    scroll(signed char v, signed char h);
    click(uint8_t b);
    release(uint8_t b);
    press(uint8_t b);
};

extern USBMouse usbMouse;

#endif
#endif

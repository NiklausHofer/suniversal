/*
    mouse handler
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

#include <Arduino.h>
#include <Mouse.h>

#include "config.h"
#include "mouse.h"

/*
	collected from Linux mouse man page:

    The Sun protocol uses 1 start bit, 8 data bits, no parity and two stop bits
    at the speed of 1200 bits/sec. Data is sent to RxD in 3-byte packets. The
    relative mouse movement is sent as dx (positive means right) and dy (positive
    means down). dx is sent as the sum of the two two's-complement values, dy is
    sent as negated sum of the two two's-complement values. lb (mb, rb) are
    cleared when the left (middle, right) button is pressed:

	 byte  d7    d6     d5     d4     d3     d2     d1     d0
	  1    1     0      0      0      0      lb     mb     rb
	  2    0    dxa6   dxa5   dxa4   dxa3   dxa2   dxa1   dxa0
	  3    0    dya6   dya5   dya4   dya3   dya2   dya1   dya0
*/

#define BUTTON_RIGHT_MASK  0x01
#define BUTTON_MIDDLE_MASK 0x02
#define BUTTON_LEFT_MASK   0x04

#define DATA_FRAME_START   0x80

MouseConverter::MouseConverter() {
	bufferIx = 0;
	Mouse.begin();
}

MouseConverter::update(uint8_t data) {
	// we need to sync on data frame start
	if ((data & DATA_FRAME_START) != 0) {
		buffer[0] = data;
		bufferIx = 1;
	} else if (bufferIx > 0) {
		buffer[bufferIx++] = data;
		if (bufferIx == array_len(buffer)) {
			handleButtons(buffer[0]);
			handleMove(buffer[1], buffer[2]);
			bufferIx = 0;
		}
	}
}

MouseConverter::handleMove(uint8_t dx, uint8_t dy) {
	if (dx != 0 || dy != 0) {
		Mouse.move(dx, dy);
	}
}

MouseConverter::handleButtons(uint8_t states) {
	if ((states ^ buttonStates) != 0) { // any changes at all?
		handleButton(states, BUTTON_LEFT_MASK, MOUSE_LEFT);
		handleButton(states, BUTTON_MIDDLE_MASK, MOUSE_MIDDLE);
		handleButton(states, BUTTON_RIGHT_MASK, MOUSE_RIGHT);
		buttonStates = states;
	}
}

MouseConverter::handleButton(uint8_t states, uint8_t buttonMask, char buttonId) {
	if (((states ^ buttonStates) & buttonMask) != 0) { // any change for button?
		if ((states & buttonMask) == 0) {
			Mouse.press(buttonId);
		} else {
			Mouse.release(buttonId);
		}
	}
}

MouseConverter mouseConverter;

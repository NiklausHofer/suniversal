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

#ifndef MOUSE_CONVERTE_h
#define MOUSE_CONVERTER_h

#include <stdint.h>

class MouseConverter {

private:
    uint8_t buttonStates;
    uint8_t buffer[5];
    uint8_t bufferIx;
    uint8_t frameLength;
    bool fiveBytes;
    flushBuffer();
    handleMove(uint8_t dx, uint8_t dy);
    handleButtons(uint8_t state);
    handleButton(uint8_t states, uint8_t buttonMask, char buttonId);

public:
    MouseConverter();
    update(uint8_t data);
};

extern MouseConverter mouseConverter;

#endif

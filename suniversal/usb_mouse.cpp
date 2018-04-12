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

#include "usb_mouse.h"

#if defined(_USING_HID)

static const uint8_t hidReportDescriptorMouse[] PROGMEM = {
    // mouse
    0x05, 0x01,         // USAGE_PAGE (Generic Desktop)  // 54
    0x09, 0x02,         // USAGE (Mouse)
    0xa1, 0x01,         // COLLECTION (Application)
    0x09, 0x01,         //   USAGE (Pointer)
    0xa1, 0x00,         //   COLLECTION (Physical)
    0x85, 0x01,         //     REPORT_ID (1)
    0x05, 0x09,         //     USAGE_PAGE (Button)
    0x19, 0x01,         //     USAGE_MINIMUM (Button 1)
    0x29, 0x03,         //     USAGE_MAXIMUM (Button 3)
    0x15, 0x00,         //     LOGICAL_MINIMUM (0)
    0x25, 0x01,         //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,         //     REPORT_COUNT (3)
    0x75, 0x01,         //     REPORT_SIZE (1)
    0x81, 0x02,         //     INPUT (Data,Var,Abs)
    0x95, 0x01,         //     REPORT_COUNT (1)
    0x75, 0x05,         //     REPORT_SIZE (5)
    0x81, 0x03,         //     INPUT (Cnst,Var,Abs)
    0x05, 0x01,         //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,         //     USAGE (X)
    0x09, 0x31,         //     USAGE (Y)
    0x09, 0x38,         //     USAGE (Wheel)
    0x15, 0x81,         //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,         //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,         //     REPORT_SIZE (8)
    0x95, 0x03,         //     REPORT_COUNT (3)
    0x81, 0x06,         //     INPUT (Data,Var,Rel)
    0x05, 0x0c,         //     USAGE PAGE (Consumer Devices)
    0x0a, 0x38, 0x02,   //     USAGE (AC Pan)
    0x15, 0x81,         //     LOGICAL_MINIMUM (-127)
    0x25, 0x7f,         //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,         //     REPORT_SIZE (8)
    0x95, 0x01,         //     REPORT_COUNT (1)
    0x81, 0x06,         //     INPUT (Data, Var, Rel)
    0xc0,               //   END_COLLECTION
    0xc0,               // END_COLLECTION
};

/*

 */
USBMouse::USBMouse() : buttons(0) {
    static HIDSubDescriptor node(hidReportDescriptorMouse,
        sizeof(hidReportDescriptorMouse));
    HID().AppendDescriptor(&node);
}

/*

 */
USBMouse::move(signed char x, signed char y) {
    send(x, y, 0, 0);
}

/*

 */
USBMouse::scroll(signed char v, signed char h) {
    send(0, 0, v, h);
}

/*

 */
USBMouse::setButtons(uint8_t b) {
    if (b != buttons) {
        buttons = b;
        send(0, 0, 0, 0);
    }
}

/*

 */
USBMouse::press(uint8_t b) {
    setButtons(buttons | b);
}

/*

 */
USBMouse::release(uint8_t b) {
    setButtons(buttons & ~b);
}

/*

 */
USBMouse::click(uint8_t b) {
    buttons = b;
    send(0, 0, 0, 0);
    buttons = 0;
    send(0, 0, 0, 0);
}

/*

 */
USBMouse::send(uint8_t x, uint8_t y, uint8_t v, uint8_t h) {
    reportData[0] = buttons;
    reportData[1] = x;
    reportData[2] = y;
    reportData[3] = v;
    reportData[4] = h;
    HID().SendReport(1, reportData, sizeof(reportData));
}

USBMouse usbMouse;

#endif

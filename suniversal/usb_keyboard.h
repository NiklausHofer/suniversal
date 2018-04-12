/*
    USB keyboard
    Copyright (c) 2018, Alexander Vollschwitz

    based on code from NicoHood's HID project, Copyright (c) 2014-2015, NicoHood
    https://github.com/NicoHood/HID

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

#ifndef USB_KEYBOARD_h
#define USB_KEYBOARD_h

#include <Arduino.h>
#include <PluggableUSB.h>
#include <HID.h>

// ---------------------------------------------------------------------------

#if ARDUINO < 10607
#error suniversal requires Arduino IDE 1.6.7 or greater.
#endif

#if !defined(USBCON)
#error suniversal can only be used with a USB MCU.
#endif

// ---------------------------------------------------------------------------

// bit masks for LED states
#define USB_LED_NUM_LOCK         1
#define USB_LED_CAPS_LOCK        1 << 1
#define USB_LED_SCROLL_LOCK      1 << 2
#define USB_LED_COMPOSE          1 << 3
#define USB_LED_KANA             1 << 4
#define USB_LED_POWER            1 << 5
#define USB_LED_SHIFT            1 << 6
#define USB_LED_DO_NOT_DISTURB   1 << 7

/*
    key report data, up to 6 keys and modifiers at once
 */
typedef struct ReportData {
    uint8_t modifiers;
    uint8_t reserved;
    uint8_t keys[6];
};

/*
    for interfacing with USB
 */
class USBKeyboard : public PluggableUSBModule {

private:
    uint8_t epType[1];
    uint8_t protocol;
    uint8_t idle;
    uint8_t leds;
    uint8_t* featureReport;
    int featureLength;
    ReportData* reportData;

protected:
    // implementation of the PUSBListNode
    int getInterface(uint8_t* interfaceCount);
    int getDescriptor(USBSetup& setup);
    bool setup(USBSetup& setup);

public:
    USBKeyboard();
    uint8_t getLeds();
    uint8_t getProtocol();
    setFeatureReport(void* report, int length);
    int availableFeatureReport();
    enableFeatureReport();
    disableFeatureReport();
    setReportData(ReportData* data);
    int send();
    wakeupHost();
};

extern USBKeyboard usbKeyboard;

#endif

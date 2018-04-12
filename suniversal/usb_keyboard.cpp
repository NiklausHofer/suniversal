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

#include "usb_keyboard.h"

static const uint8_t hidReportDescriptorKeyboard[] PROGMEM = {
    //  Keyboard
    0x05, 0x01,                      /* USAGE_PAGE (Generic Desktop) 47 */
    0x09, 0x06,                      /* USAGE (Keyboard) */
    0xa1, 0x01,                      /* COLLECTION (Application) */
    0x05, 0x07,                      /*   USAGE_PAGE (Keyboard) */

    /* Keyboard Modifiers (shift, alt, ...) */
    0x19, 0xe0,                      /*   USAGE_MINIMUM (Keyboard LeftControl) */
    0x29, 0xe7,                      /*   USAGE_MAXIMUM (Keyboard Right GUI) */
    0x15, 0x00,                      /*   LOGICAL_MINIMUM (0) */
    0x25, 0x01,                      /*   LOGICAL_MAXIMUM (1) */
    0x75, 0x01,                      /*   REPORT_SIZE (1) */
    0x95, 0x08,                      /*   REPORT_COUNT (8) */
    0x81, 0x02,                      /*   INPUT (Data,Var,Abs) */

    /* Reserved byte, used for consumer reports, only works with linux */
    0x05, 0x0C,                      /*   Usage Page (Consumer) */
    0x95, 0x01,                      /*   REPORT_COUNT (1) */
    0x75, 0x08,                      /*   REPORT_SIZE (8) */
    0x15, 0x00,                      /*   LOGICAL_MINIMUM (0) */
    0x26, 0xFF, 0x00,                /*   LOGICAL_MAXIMUM (255) */
    0x19, 0x00,                      /*   USAGE_MINIMUM (0) */
    0x29, 0xFF,                      /*   USAGE_MAXIMUM (255) */
    0x81, 0x00,                      /*   INPUT (Data,Ary,Abs) */

    /* 5 LEDs for num lock etc, 3 left for advanced, custom usage */
    0x05, 0x08,                      /*   USAGE_PAGE (LEDs) */
    0x19, 0x01,                      /*   USAGE_MINIMUM (Num Lock) */
    0x29, 0x08,                      /*   USAGE_MAXIMUM (Kana + 3 custom)*/
    0x95, 0x08,                      /*   REPORT_COUNT (8) */
    0x75, 0x01,                      /*   REPORT_SIZE (1) */
    0x91, 0x02,                      /*   OUTPUT (Data,Var,Abs) */

    /* 6 Keyboard keys */
    0x05, 0x07,                      /*   USAGE_PAGE (Keyboard) */
    0x95, 0x06,                      /*   REPORT_COUNT (6) */
    0x75, 0x08,                      /*   REPORT_SIZE (8) */
    0x15, 0x00,                      /*   LOGICAL_MINIMUM (0) */
    0x26, 0xE7, 0x00,                /*   LOGICAL_MAXIMUM (231) */
    0x19, 0x00,                      /*   USAGE_MINIMUM (Reserved (no event indicated)) */
    0x29, 0xE7,                      /*   USAGE_MAXIMUM (Keyboard Right GUI) */
    0x81, 0x00,                      /*   INPUT (Data,Ary,Abs) */

    /* End */
    0xc0                            /* END_COLLECTION */
};

/*

 */
USBKeyboard::USBKeyboard() :
    PluggableUSBModule(1, 1, epType),
    protocol(HID_REPORT_PROTOCOL),
    idle(1),
    leds(0),
    featureReport(NULL),
    featureLength(0),
    reportData(NULL)
{
    epType[0] = EP_TYPE_INTERRUPT_IN;
    PluggableUSB().plug(this);
}

/*
    returns the number of bytes sent and increments the interfaceNum
    variable with the number of interfaces used
 */
int USBKeyboard::getInterface(uint8_t* interfaceCount) {
    *interfaceCount += 1; // uses 1
    HIDDescriptor hidInterface = {
        D_INTERFACE(
            pluggedInterface, 1,
            USB_DEVICE_CLASS_HUMAN_INTERFACE,
            HID_SUBCLASS_BOOT_INTERFACE,
            HID_PROTOCOL_KEYBOARD),
        D_HIDREPORT(sizeof(hidReportDescriptorKeyboard)),
        D_ENDPOINT(
            USB_ENDPOINT_IN(pluggedEndpoint),
            USB_ENDPOINT_TYPE_INTERRUPT,
            USB_EP_SIZE, 0x01)
    };
    return USB_SendControl(0, &hidInterface, sizeof(hidInterface));
}

/*
    returns the number of bytes sent if the request was directed to the
    module, 0 if the request has not been served, or -1 if errors have
    been encountered
 */
int USBKeyboard::getDescriptor(USBSetup& setup) {
    // Check if this is a HID Class Descriptor request and HID Class
    // Descriptor wIndex contains the interface number
    if (setup.bmRequestType != REQUEST_DEVICETOHOST_STANDARD_INTERFACE ||
        setup.wValueH != HID_REPORT_DESCRIPTOR_TYPE ||
        setup.wIndex != pluggedInterface) {
        return 0;
    }
    // Reset the protocol on reenumeration. Normally, the host should not
    // assume the state of the protocol according to the USB specs, but
    // Windows and Linux just assume its in report mode.
    protocol = HID_REPORT_PROTOCOL;

    return USB_SendControl(TRANSFER_PGM,
        hidReportDescriptorKeyboard, sizeof(hidReportDescriptorKeyboard));
}

/*
    returns true if the request was directed to the module and executed
    correctly, false otherwise
 */
bool USBKeyboard::setup(USBSetup& setup) {

    if (pluggedInterface != setup.wIndex) {
        return false;
    }

    uint8_t request = setup.bRequest;
    uint8_t requestType = setup.bmRequestType;

    if (requestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE) {
        if (request == HID_GET_REPORT) {
            // TODO: not quite sure whether this is correct, check page 51 of
            //  http://www.usb.org/developers/hidpage/HID1_11.pdf
            //
            return send() > 0;
        }
        if (request == HID_GET_PROTOCOL) {
            // TODO improve
            UEDATX = protocol;
            return true;
        }
        if (request == HID_GET_IDLE) {
            // TODO improve
            UEDATX = idle;
            return true;
        }
    }

    if (requestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE) {

        if (request == HID_SET_PROTOCOL) {
            protocol = setup.wValueL;
            return true;
        }

        if (request == HID_SET_IDLE) {
            idle = setup.wValueL;
            return true;
        }

        if (request == HID_SET_REPORT) {

            // Check if data has the correct length afterwards
            int length = setup.wLength;

            // Feature (set feature report)
            if (setup.wValueH == HID_REPORT_TYPE_FEATURE) {
                // No need to check for negative featureLength values,
                // except the host tries to send more then 32k bytes.
                // We dont have that much ram anyways.
                if (length == featureLength) {
                    USB_RecvControl(featureReport, featureLength);
                    // Block until data is read (make length negative)
                    disableFeatureReport();
                    return true;
                }
                // TODO fake clear data?

            } else if (setup.wValueH == HID_REPORT_TYPE_OUTPUT) {
                // Output (set led states)
                if (length == sizeof(leds)){
                    USB_RecvControl(&leds, length);
                    return true;
                }

            } else if (setup.wValueH == HID_REPORT_TYPE_INPUT) {
                // Input (set HID report)
                if (length == sizeof(ReportData)) {
                    USB_RecvControl(&reportData, length);
                    return true;
                }
            }
        }
    }

    return false;
}

/*

 */
uint8_t USBKeyboard::getLeds() {
    return leds;
}

/*

 */
uint8_t USBKeyboard::getProtocol() {
    return protocol;
}

/*

 */
USBKeyboard::setFeatureReport(void* report, int length){
    if(length > 0){
        featureReport = (uint8_t*)report;
        featureLength = length;
        // Disable feature report by default
        disableFeatureReport();
    }
}

/*

 */
int USBKeyboard::availableFeatureReport() {
    if(featureLength < 0){
        return featureLength & ~0x8000;
    }
    return 0;
}

/*

 */
USBKeyboard::enableFeatureReport() {
    featureLength &= ~0x8000;
}

/*

 */
USBKeyboard::disableFeatureReport() {
    featureLength |= 0x8000;
}

/*

 */
USBKeyboard::setReportData(ReportData* data) {
    reportData = data;
}

/*

 */
int USBKeyboard::send() {
    return reportData != NULL ?
        USB_Send(pluggedEndpoint | TRANSFER_RELEASE,
            reportData, sizeof(ReportData)) : 0;
}

/*

 */
USBKeyboard::wakeupHost() {
    USBDevice.wakeupHost();
}

USBKeyboard usbKeyboard;

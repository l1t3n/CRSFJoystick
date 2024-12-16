/*
 * Copyright (c) 2018-2019, Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "stdint.h"
#include "PluggableUSBHID.h"
#include "PicoGamepad.h"
#include "usb_phy_api.h"

#define REPORT_ID_KEYBOARD 1
#define REPORT_ID_VOLUME 3

using namespace arduino;

PicoGamepad::PicoGamepad(bool connect, uint16_t vendor_id, uint16_t product_id, uint16_t product_release) : USBHID(get_usb_phy(), 0, 0, vendor_id, product_id, product_release)
{ }

PicoGamepad::PicoGamepad(USBPhy *phy, uint16_t vendor_id, uint16_t product_id, uint16_t product_release) : USBHID(phy, 0, 0, vendor_id, product_id, product_release)
{ }

PicoGamepad::~PicoGamepad()
{ }

const uint8_t *PicoGamepad::report_desc()
{
    static const uint8_t reportDescriptor[] = {
        0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
        0x09, 0x05,                    //     USAGE (Game Pad)
        0xa1, 0x01,                    //     COLLECTION (Application)
        0xa1, 0x00,                    //       COLLECTION (Physical)
        0x05, 0x09,                    //         USAGE_PAGE (Button)
        0x19, 0x01,                    //         USAGE_MINIMUM (Button 1)
        0x29, 0x18,                    //         USAGE_MAXIMUM (Button 24)
        0x15, 0x00,                    //         LOGICAL_MINIMUM (0)
        0x25, 0x01,                    //         LOGICAL_MAXIMUM (1)
        0x95, 0x18,                    //         REPORT_COUNT (24)
        0x75, 0x01,                    //         REPORT_SIZE (1)
        0x81, 0x02,                    //         INPUT (Data,Var,Abs)
        0x05, 0x01,                    //         USAGE_PAGE (Generic Desktop)
        0x09, 0x30,                    //         USAGE (X)
        0x09, 0x31,                    //         USAGE (Y)
        0x09, 0x32,                    //         USAGE (Z)
        0x09, 0x33,                    //         USAGE (Rx)
        0x09, 0x34,                    //         USAGE (Ry)
        0x09, 0x35,                    //         USAGE (Rz)
        0x09, 0x36,                    //         USAGE (Slider)
        0x09, 0x37,                    //         USAGE (Slider)
        0x16, 0x00, 0x00,              //         LOGICAL_MINIMUM (0)
        0x26, 0xFF, 0x07,              //         LOGICAL_MAXIMUM (2047)
        0x75, 0x10,                    //         REPORT_SIZE (16)
        0x95, 0x08,                    //         REPORT_COUNT (8)
        0x81, 0x02,                    //         INPUT (Data,Var,Abs)
        0xc0,                          //       END_COLLECTION
        0xc0                           //     END_COLLECTION
    };
    reportLength = sizeof(reportDescriptor);
    return reportDescriptor;
}


void PicoGamepad::SetButton(int idx, bool val)
{
    if (idx > 24 || idx < 0)
    {
        return;
    }
    bitWrite(inputArray[idx / 8], idx % 8, val);
}


void PicoGamepad::SetAxis0(uint16_t val)
{
    inputArray[AXIS0_LSB] = LSB(val);
    inputArray[AXIS0_MSB] = MSB(val);
}
void PicoGamepad::SetAxis1(uint16_t val)
{
    inputArray[AXIS1_LSB] = LSB(val);
    inputArray[AXIS1_MSB] = MSB(val);
}
void PicoGamepad::SetAxis2(uint16_t val)
{
    inputArray[AXIS2_LSB] = LSB(val);
    inputArray[AXIS2_MSB] = MSB(val);
}
void PicoGamepad::SetAxis3(uint16_t val)
{
    inputArray[AXIS3_LSB] = LSB(val);
    inputArray[AXIS3_MSB] = MSB(val);
}
void PicoGamepad::SetAxis4(uint16_t val)
{
    inputArray[AXIS4_LSB] = LSB(val);
    inputArray[AXIS4_MSB] = MSB(val);
}
void PicoGamepad::SetAxis5(uint16_t val)
{
    inputArray[AXIS5_LSB] = LSB(val);
    inputArray[AXIS5_MSB] = MSB(val);
}
void PicoGamepad::SetAxis6(uint16_t val)
{
    inputArray[AXIS6_LSB] = LSB(val);
    inputArray[AXIS6_MSB] = MSB(val);
}
void PicoGamepad::SetAxis7(uint16_t val)
{
    inputArray[AXIS7_LSB] = LSB(val);
    inputArray[AXIS7_MSB] = MSB(val);
}


bool PicoGamepad::send_update()
{
    _mutex.lock();

    HID_REPORT report;

    for (int i = 0; i < 19; i++)
    {
        report.data[i] = inputArray[i];
    }

    report.length = 19;

    if (!send(&report))
    {
        _mutex.unlock();
        return false;
    }

    _mutex.unlock();
    return true;
}

#define DEFAULT_CONFIGURATION (1)
#define TOTAL_DESCRIPTOR_LENGTH ((1 * CONFIGURATION_DESCRIPTOR_LENGTH) + (1 * INTERFACE_DESCRIPTOR_LENGTH) + (1 * HID_DESCRIPTOR_LENGTH) + (2 * ENDPOINT_DESCRIPTOR_LENGTH))

const uint8_t *PicoGamepad::configuration_desc(uint8_t index)
{
    if (index != 0)
    {
        return NULL;
    }
    uint8_t configuration_descriptor_temp[] = {
        CONFIGURATION_DESCRIPTOR_LENGTH, // bLength
        CONFIGURATION_DESCRIPTOR,        // bDescriptorType
        LSB(TOTAL_DESCRIPTOR_LENGTH),    // wTotalLength (LSB)
        MSB(TOTAL_DESCRIPTOR_LENGTH),    // wTotalLength (MSB)
        0x01,                            // bNumInterfaces
        DEFAULT_CONFIGURATION,           // bConfigurationValue
        0x00,                            // iConfiguration
        C_RESERVED | C_SELF_POWERED,     // bmAttributes
        C_POWER(0),                      // bMaxPower

        INTERFACE_DESCRIPTOR_LENGTH, // bLength
        INTERFACE_DESCRIPTOR,        // bDescriptorType
        0x00,                        // bInterfaceNumber
        0x00,                        // bAlternateSetting
        0x02,                        // bNumEndpoints
        HID_CLASS,                   // bInterfaceClass
        HID_SUBCLASS_BOOT,           // bInterfaceSubClass
        HID_PROTOCOL_KEYBOARD,       // bInterfaceProtocol
        0x00,                        // iInterface

        HID_DESCRIPTOR_LENGTH,                // bLength
        HID_DESCRIPTOR,                       // bDescriptorType
        LSB(HID_VERSION_1_11),                // bcdHID (LSB)
        MSB(HID_VERSION_1_11),                // bcdHID (MSB)
        0x00,                                 // bCountryCode
        0x01,                                 // bNumDescriptors
        REPORT_DESCRIPTOR,                    // bDescriptorType
        (uint8_t)(LSB(report_desc_length())), // wDescriptorLength (LSB)
        (uint8_t)(MSB(report_desc_length())), // wDescriptorLength (MSB)

        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _int_in,                    // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (MSB)
        1,                          // bInterval (milliseconds)

        ENDPOINT_DESCRIPTOR_LENGTH, // bLength
        ENDPOINT_DESCRIPTOR,        // bDescriptorType
        _int_out,                   // bEndpointAddress
        E_INTERRUPT,                // bmAttributes
        LSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (LSB)
        MSB(MAX_HID_REPORT_SIZE),   // wMaxPacketSize (MSB)
        1,                          // bInterval (milliseconds)
    };
    MBED_ASSERT(sizeof(configuration_descriptor_temp) == sizeof(_configuration_descriptor));
    memcpy(_configuration_descriptor, configuration_descriptor_temp, sizeof(_configuration_descriptor));
    return _configuration_descriptor;
}

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

#ifndef PicoGamepad_H
#define PicoGamepad_H

#include "PluggableUSBHID.h"
#include "platform/Stream.h"
#include "PlatformMutex.h"

// values addresses
#define BTN0_7 0
#define BTN8_15 1
#define BTN16_23 2
#define AXIS0_LSB 3
#define AXIS0_MSB 4
#define AXIS1_LSB 5
#define AXIS1_MSB 6
#define AXIS2_LSB 7
#define AXIS2_MSB 8
#define AXIS3_LSB 9
#define AXIS3_MSB 10
#define AXIS4_LSB 11
#define AXIS4_MSB 12
#define AXIS5_LSB 13
#define AXIS5_MSB 14
#define AXIS6_LSB 15
#define AXIS6_MSB 16
#define AXIS7_LSB 17
#define AXIS7_MSB 18

namespace arduino
{

    class PicoGamepad : public USBHID
    {
    public:
        // Array of 8 bit values which holds all inputs to be passed via HID

        /**
    * Basic constructor
    *
    * Construct this object optionally connecting and blocking until it is ready.
    *
    * @note Do not use this constructor in derived classes.
    *
    * @param connect_blocking true to perform a blocking connect, false to start in a disconnected state
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
        PicoGamepad(bool connect_blocking = true, uint16_t vendor_id = 0x1235, uint16_t product_id = 0x0050, uint16_t product_release = 0x0001);

        /**
    * Fully featured constructor
    *
    * Construct this object with the supplied USBPhy and parameters. The user
    * this object is responsible for calling connect() or init().
    *
    * @note Derived classes must use this constructor and call init() or
    * connect() themselves. Derived classes should also call deinit() in
    * their destructor. This ensures that no interrupts can occur when the
    * object is partially constructed or destroyed.
    *
    * @param phy USB phy to use
    * @param vendor_id Your vendor_id
    * @param product_id Your product_id
    * @param product_release Your product_release
    */
        PicoGamepad(USBPhy *phy, uint16_t vendor_id = 0x1235, uint16_t product_id = 0x0050, uint16_t product_release = 0x0001);

        /**
     * Destroy this object
     *
     * Any classes which inherit from this class must call deinit
     * before this destructor runs.
     */
        virtual ~PicoGamepad();

        /**
    * To send a character defined by a modifier(CTRL, SHIFT, ALT) and the key
    *
    * @code
    * //To send CTRL + s (save)
    *  keyboard.key_code('s', KEY_CTRL);
    * @endcode
    *
    * @param modifier bit 0: KEY_CTRL, bit 1: KEY_SHIFT, bit 2: KEY_ALT (default: 0)
    * @param key character to send
    * @returns true if there is no error, false otherwise
    */

        void SetButton(int idx, bool val);
 
        void SetAxis0(uint16_t val);
        void SetAxis1(uint16_t val);
        void SetAxis2(uint16_t val);
        void SetAxis3(uint16_t val);
        void SetAxis4(uint16_t val);
        void SetAxis5(uint16_t val);
        void SetAxis6(uint16_t val);
        void SetAxis7(uint16_t val);

        bool send_update();
        /*
    * To define the report descriptor. Warning: this method has to store the length of the report descriptor in reportLength.
    *
    * @returns pointer to the report descriptor
    */
        virtual const uint8_t *report_desc();

    protected:
        /*
    * Get configuration descriptor
    *
    * @returns pointer to the configuration descriptor
    */
        virtual const uint8_t *configuration_desc(uint8_t index);

    private:
        uint8_t inputArray[19];

        uint8_t _configuration_descriptor[41];
        PlatformMutex _mutex;
    };
}

#endif

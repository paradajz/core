/*
    Copyright 2017-2022 Igor Petrovic

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
    sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
    OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#pragma once

#include <inttypes.h>

#ifndef __cplusplus
typedef uint16_t char16_t;
#endif

namespace core::mcu::usb
{
    struct __attribute__((packed)) descriptorHeader_t
    {
        uint8_t size;
        uint8_t type;
    };

    struct __attribute__((packed)) descriptorConfigurationHeader_t
    {
        descriptorHeader_t header;
        uint16_t           totalConfigurationSize;
        uint8_t            totalInterfaces;
        uint8_t            configurationNumber;
        uint8_t            configurationStrIndex;
        uint8_t            configAttributes;
        uint8_t            maxPowerConsumption;
    };

    struct __attribute__((packed)) descriptorInterface_t
    {
        descriptorHeader_t header;
        uint8_t            interfaceNumber;
        uint8_t            alternateSetting;
        uint8_t            totalEndpoints;
        uint8_t            classId;
        uint8_t            subClassId;
        uint8_t            protocol;
        uint8_t            interfaceStrIndex;
    };

    struct __attribute__((packed)) descriptorInterfaceAssociation_t
    {
        descriptorHeader_t header;
        uint8_t            firstInterfaceIndex;
        uint8_t            totalInterfaces;
        uint8_t            classId;
        uint8_t            subClassId;
        uint8_t            protocol;
        uint8_t            iadStrIndex;
    };

    struct __attribute__((packed)) descriptorEndpoint_t
    {
        descriptorHeader_t header;
        uint8_t            endpointAddress;
        uint8_t            attributes;
        uint16_t           endpointSize;
        uint8_t            pollingIntervalMs;
    };

    struct __attribute__((packed)) descriptorDevice_t
    {
        descriptorHeader_t header;
        uint16_t           usbSpecification;
        uint8_t            classId;
        uint8_t            subClassId;
        uint8_t            protocol;
        uint8_t            endpoint0Size;
        uint16_t           vendorId;
        uint16_t           productId;
        uint16_t           releaseNumber;
        uint8_t            manufacturerStrIndex;
        uint8_t            productStrIndex;
        uint8_t            serialNumStrIndex;
        uint8_t            numberOfConfigurations;
    };

    struct descriptorString_t
    {
        descriptorHeader_t header;
        char16_t           unicodeString[];
    };

    struct descriptorUIDString_t
    {
        descriptorHeader_t header;
        uint16_t           unicodeString[CORE_MCU_UID_BITS / 4];
    };

    enum descriptorTypes_t
    {
        DESC_TYPE_DEVICE                = 0x01,
        DESC_TYPE_CONFIGURATION         = 0x02,
        DESC_TYPE_STRING                = 0x03,
        DESC_TYPE_INTERFACE             = 0x04,
        DESC_TYPE_ENDPOINT              = 0x05,
        DESC_TYPE_DEVICE_QUALIFIER      = 0x06,
        DESC_TYPE_OTHER                 = 0x07,
        DESC_TYPE_INTERFACE_POWER       = 0x08,
        DESC_TYPE_INTERFACE_ASSOCIATION = 0x0B,
    };

    enum descriptorClassSubclassProtocol_t
    {
        CSCP_NO_DEVICE_CLASS          = 0x00,
        CSCP_NO_DEVICE_SUBCLASS       = 0x00,
        CSCP_NO_DEVICE_PROTOCOL       = 0x00,
        CSCP_VENDOR_SPECIFIC_CLASS    = 0xFF,
        CSCP_VENDOR_SPECIFIC_SUBCLASS = 0xFF,
        CSCP_VENDOR_SPECIFIC_PROTOCOL = 0xFF,
        CSCP_IAD_DEVICE_CLASS         = 0xEF,
        CSCP_IAD_DEVICE_SUBCLASS      = 0x02,
        CSCP_IAD_DEVICE_PROTOCOL      = 0x01,
    };
}    // namespace core::mcu::usb

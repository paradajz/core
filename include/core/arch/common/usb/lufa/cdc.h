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

#include "common.h"

namespace core::mcu::usb
{
    enum CDC_ClassRequests_t
    {
        CDC_REQ_SEND_ENCAPSUALTED_COMMAND = 0x00,
        CDC_REQ_GET_ENCAPSULATED_RESPONSE = 0x01,
        CDC_REQ_SET_LINE_ENCODING         = 0x20,
        CDC_REQ_GET_LINE_ENCODING         = 0x21,
        CDC_REQ_SET_CONTROL_LINE_STATE    = 0x22,
        CDC_REQ_SEND_BREAK                = 0x23,
    };

    enum CDC_DescriptorSubtypes_t
    {
        CDC_DSUBTYPE_CS_INTERFACE_HEADER             = 0x00,
        CDC_DSUBTYPE_CS_INTERFACE_CALL_MANAGEMENT    = 0x01,
        CDC_DSUBTYPE_CS_INTERFACE_ACM                = 0x02,
        CDC_DSUBTYPE_CS_INTERFACE_DIRECT_LINE        = 0x03,
        CDC_DSUBTYPE_CS_INTERFACE_TELEPHONE_RINGER   = 0x04,
        CDC_DSUBTYPE_CS_INTERFACE_TELEPHONE_CALL     = 0x05,
        CDC_DSUBTYPE_CS_INTERFACE_UNION              = 0x06,
        CDC_DSUBTYPE_CS_INTERFACE_COUNTRY_SELECTION  = 0x07,
        CDC_DSUBTYPE_CS_INTERFACE_TELEPHONE_OP_MODES = 0x08,
        CDC_DSUBTYPE_CS_INTERFACE_USB_TERMINAL       = 0x09,
        CDC_DSUBTYPE_CS_INTERFACE_NETWORK_CHANNEL    = 0x0A,
        CDC_DSUBTYPE_CS_INTERFACE_PROTOCOL_UNIT      = 0x0B,
        CDC_DSUBTYPE_CS_INTERFACE_EXTENSION_UNIT     = 0x0C,
        CDC_DSUBTYPE_CS_INTERFACE_MULTI_CHANNEL      = 0x0D,
        CDC_DSUBTYPE_CS_INTERFACE_CAPI               = 0x0E,
        CDC_DSUBTYPE_CS_INTERFACE_ETHERNET           = 0x0F,
        CDC_DSUBTYPE_CS_INTERFACE_ATM                = 0x10,
    };

    enum CDC_Descriptor_ClassSubclassProtocol_t
    {
        CDC_CSCP_CDC_CLASS                = 0x02,
        CDC_CSCP_NO_SPECIFIC_SUBCLASS     = 0x00,
        CDC_CSCP_ACM_SUBCLASS             = 0x02,
        CDC_CSCP_AT_COMMAND_PROTOCOL      = 0x01,
        CDC_CSCP_NO_SPECIFIC_PROTOCOL     = 0x00,
        CDC_CSCP_VENDOR_SPECIFIC_PROTOCOL = 0xFF,
        CDC_CSCP_CDC_DATA_CLASS           = 0x0A,
        CDC_CSCP_NO_DATA_SUBCLASS         = 0x00,
        CDC_CSCP_NO_DATA_PROTOCOL         = 0x00,
    };

    enum CDC_DescriptorTypes_t
    {
        CDC_DTYPE_CS_INTERFACE = 0x24,
        CDC_DTYPE_CS_ENDPOINT  = 0x25,
    };

    struct __attribute__((packed)) cdcDescriptorFunctionalHeader_t
    {
        descriptorHeader_t header           = {};
        uint8_t            subType          = 0;
        uint16_t           cdcSpecification = 0;
    };

    struct __attribute__((packed)) cdcDescriptorFunctionalACM_t
    {
        descriptorHeader_t header       = {};
        uint8_t            subType      = 0;
        uint8_t            capabilities = 0;
    };

    struct __attribute__((packed)) cdcDescriptorFunctionalUnion_t
    {
        descriptorHeader_t header                = {};
        uint8_t            subType               = 0;
        uint8_t            masterInterfaceNumber = 0;
        uint8_t            slaveInterfaceNumber  = 0;
    };
}    // namespace core::mcu::usb

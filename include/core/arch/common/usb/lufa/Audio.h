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
#include "Common.h"

namespace core::mcu::usb
{
    enum audioDescriptorClassSubclassProtocol_t
    {
        AUDIO_CSCP_AUDIO_CLASS              = 0x01,
        AUDIO_CSCP_CONTROL_SUBCLASS         = 0x01,
        AUDIO_CSCP_CONTROL_PROTOCOL         = 0x00,
        AUDIO_CSCP_AUDIO_STREAMING_SUBCLASS = 0x02,
        AUDIO_CSCP_MIDI_STREAMING_SUBCLASS  = 0x03,
        AUDIO_CSCP_STREAMING_PROTOCOL       = 0x00,
    };

    enum audioDescriptorTypes_t
    {
        AUDIO_DTYPE_CS_INTERFACE = 0x24,
        AUDIO_DTYPE_CS_ENDPOINT  = 0x25,
    };

    enum audioCSInterfaceACSubTypes_t
    {
        AUDIO_DSUBTYPE_CS_INTERFACE_HEADER          = 0x01,
        AUDIO_DSUBTYPE_CS_INTERFACE_INPUT_TERMINAL  = 0x02,
        AUDIO_DSUBTYPE_CS_INTERFACE_OUTPUT_TERMINAL = 0x03,
        AUDIO_DSUBTYPE_CS_INTERFACE_MIXER           = 0x04,
        AUDIO_DSUBTYPE_CS_INTERFACE_SELECTOR        = 0x05,
        AUDIO_DSUBTYPE_CS_INTERFACE_FEATURE         = 0x06,
        AUDIO_DSUBTYPE_CS_INTERFACE_PROCESSING      = 0x07,
        AUDIO_DSUBTYPE_CS_INTERFACE_EXTENSION       = 0x08,
    };

    enum audioCSInterfaceASSubTypes_t
    {
        AUDIO_DSUBTYPE_CS_INTERFACE_GENERAL         = 0x01,
        AUDIO_DSUBTYPE_CS_INTERFACE_FORMAT_TYPE     = 0x02,
        AUDIO_DSUBTYPE_CS_INTERFACE_FORMAT_SPECIFIC = 0x03,
    };

    enum audioCSEndpointSubTypes_t
    {
        AUDIO_DSUBTYPE_CS_ENDPOINT_GENERAL = 0x01,
    };

    struct __attribute__((packed)) audioDescriptorInterfaceAC_t
    {
        descriptorHeader_t header;
        uint8_t            subType;
        uint16_t           acSpecification;
        uint16_t           totalLength;
        uint8_t            inCollection;
        uint8_t            interfaceNumber;
    };

    struct __attribute__((packed)) audioDescriptorStreamEndpointStd_t
    {
        descriptorEndpoint_t endpoint;
        uint8_t              refresh;
        uint8_t              syncEndpointNumber;
    };
}    // namespace core::mcu::usb

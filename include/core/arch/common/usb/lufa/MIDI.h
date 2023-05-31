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
    enum midiJackTypes_t
    {
        MIDI_JACKTYPE_EMBEDDED = 0x01,
        MIDI_JACKTYPE_EXTERNAL = 0x02,
    };

    struct __attribute__((packed)) midiDescriptorAudioInterfaceAS_t
    {
        descriptorHeader_t header             = {};
        uint8_t            subType            = 0;
        uint16_t           audioSpecification = 0;
        uint16_t           totalLength        = 0;
    };

    struct __attribute__((packed)) midiDescriptorInputJack_t
    {
        descriptorHeader_t header       = {};
        uint8_t            subType      = 0;
        uint8_t            jackType     = 0;
        uint8_t            jackId       = 0;
        uint8_t            jackStrIndex = 0;
    };

    struct __attribute__((packed)) midiDescriptorOutputJack_t
    {
        descriptorHeader_t header          = {};
        uint8_t            subType         = 0;
        uint8_t            jackType        = 0;
        uint8_t            jackId          = 0;
        uint8_t            numberOfPins    = 0;
        uint8_t            sourceJackId[1] = {};
        uint8_t            sourcePinId[1]  = {};
        uint8_t            jackStrIndex    = 0;
    };

    struct __attribute__((packed)) midiDescriptorJackEndpoint_t
    {
        descriptorHeader_t header              = {};
        uint8_t            subType             = 0;
        uint8_t            totalEmbeddedJacks  = 0;
        uint8_t            associatedJackId[1] = {};
    };
}    // namespace core::mcu::usb

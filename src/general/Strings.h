/*
    Copyright 2017 Igor Petroviæ

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

#if defined(USE_STRINGS) || defined(__DOXYGEN__)

///
/// \defgroup coreGeneralStrings Strings
/// \ingroup coreGeneral
/// \brief C++ class used for easier building of strings.
/// @{

#ifndef STRING_BUFFER_SIZE
#error STRING_BUFFER_SIZE undefined
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class StringBuffer
{
    public:
    ///
    /// \brief Default constructor (empty).
    ///
    StringBuffer()
    {
        
    }

    ///
    /// \brief Appends single number to stringBuffer.
    /// @param [in] number              Number to append.
    ///
    void addNumberToCharArray(int32_t number)
    {
        char intToCharArray[7];
        itoa(number, intToCharArray, 10);
        stringSize += strlen(intToCharArray);

        strcat(stringBuffer, intToCharArray);
        stringBuffer[stringSize] = '\0';
    }

    ///
    /// \brief Appends raw byte value to stringBuffer.
    /// @param [in] byte                Byte value to append.
    ///
    void addByteToCharArray(uint8_t byte)
    {
        stringBuffer[stringSize] = byte;
        stringSize++;
        stringBuffer[stringSize] = '\0';
    }

    ///
    /// \brief Appends text to stringBuffer.
    /// @param [in] text                Text to append.
    ///
    void appendText(const char *text)
    {
        stringSize += strlen(text);
        strcat(stringBuffer, text);
        stringBuffer[stringSize] = '\0';
    }

    ///
    /// \brief Appends space to stringBuffer.
    /// @param [in] numberOfSpaces      Number of spaces to append to stringBuffer.
    ///
    void addSpaceToCharArray(uint8_t numberOfSpaces)
    {
        for (int i = 0; i < numberOfSpaces; i++)
            stringBuffer[stringSize + i] = ' ';

        stringSize += numberOfSpaces;
        stringBuffer[stringSize] = '\0';
    }

    ///
    /// \brief Clears contents of stringBuffer.
    ///
    void startLine()
    {
        stringBuffer[0] = '\0';
    }

    ///
    /// \brief Ends stringBuffer by appending '\0' char to current index.
    ///
    void endLine()
    {
        stringBuffer[stringSize] = '\0';
    }

    private:
    ///
    /// \brief Holds current size of string.
    ///
    uint8_t stringSize;

    ///
    /// \brief String buffer used by all string functions.
    ///
    char stringBuffer[STRING_BUFFER_SIZE];
};


/// @}

#endif
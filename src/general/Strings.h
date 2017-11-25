/*
    Copyright 2017 Igor Petrovi�

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

///
/// \defgroup coreGeneralStrings Strings
/// \ingroup coreGeneral
/// \brief C++ class used for easier building of strings.
/// @{

#ifndef STRING_BUFFER_SIZE
#warning Please define STRING_BUFFER_SIZE. Setting size to 1 byte.
#define STRING_BUFFER_SIZE 1
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
    /// \brief String buffer used by all string functions.
    ///
    char buffer[STRING_BUFFER_SIZE];

    ///
    /// \brief Appends integer to string (int32_t).
    /// @param [in] number  Number to append.
    ///
    void appendInt(int32_t number)
    {
        char intToCharArray[7];
        itoa(number, intToCharArray, 10);
        stringSize += strlen(intToCharArray);

        strcat(buffer, intToCharArray);
        buffer[stringSize] = '\0';
    }

    ///
    /// \brief Appends text to string.
    /// @param [in] text    Text to append.
    ///
    void appendText(const char *text)
    {
        stringSize += strlen(text);
        strcat(buffer, text);
        buffer[stringSize] = '\0';
    }

    ///
    /// \brief Appends space to string.
    /// @param [in] numberOfSpaces  Number of spaces to append.
    ///
    void addSpaceToCharArray(uint8_t numberOfSpaces)
    {
        for (int i = 0; i < numberOfSpaces; i++)
            buffer[stringSize + i] = ' ';

        stringSize += numberOfSpaces;
        buffer[stringSize] = '\0';
    }

    ///
    /// \brief Converts given integer to string.
    /// @param [in] number  Number to convert (int32_t).
    ///
    void intToString(int32_t number)
    {
        itoa(number, buffer, 10);
        stringSize = strlen(buffer);
    }

    ///
    /// \brief Concatenates given text to current string.
    /// @param [in] text    Text to Concatenate.
    ///
    void merge(char* text)
    {
        strcat(buffer, text);
        //make sure to update string size after merging
        stringSize += strlen(text);
    }

    ///
    /// \brief Clears entire string.
    ///
    void startLine()
    {
        buffer[0] = '\0';
    }

    ///
    /// \brief Ends string by appending '\0' character to current index.
    ///
    void endLine()
    {
        buffer[stringSize] = '\0';
    }

    ///
    /// \brief Returns current string.
    /// \returns Current string.
    ///
    char* getString()
    {
        return buffer;
    }

    private:
    ///
    /// \brief Holds current size of string.
    ///
    uint8_t stringSize;
};


/// @}

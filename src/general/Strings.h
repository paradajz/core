/*
    Copyright 2017-2018 Igor Petrovic

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

#ifdef STRING_BUFFER_SIZE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

class StringBuffer
{
    public:
    ///
    /// \brief Default constructor (empty).
    ///
    StringBuffer() {}

    ///
    /// \brief Appends integer to string (int32_t).
    /// @param [in] number  Number to append.
    ///
    void appendInt(int32_t number)
    {
        char intToCharArray[7];
        itoa(number, intToCharArray, 10);

        if ((stringSize + strlen(intToCharArray)) >= (STRING_BUFFER_SIZE-1))
            return; //overflow

        strcat(buffer, intToCharArray);
        stringSize += strlen(intToCharArray);
        buffer[stringSize] = '\0';
    }

    ///
    /// \brief Appends text to string.
    /// @param [in] text    Text to append.
    ///
    void appendText(const char *text)
    {
        if ((stringSize + strlen(text)) >= (STRING_BUFFER_SIZE-1))
            return; //overflow

        strcat(buffer, text);
        stringSize += strlen(text);
        buffer[stringSize] = '\0';
    }

    #ifdef __AVR__
    ///
    /// \brief Appends text located in flash memory to string.
    /// @param [in] text    Text to append.
    ///
    void appendText_P(const char *text PROGMEM)
    {
        if ((stringSize + strlen_P(text)) >= (STRING_BUFFER_SIZE-1))
            return; //overflow

        strcat_P(buffer, text);
        stringSize += strlen_P(text);
        buffer[stringSize] = '\0';
    }
    #endif

    ///
    /// \brief Appends character to string.
    /// @param [in] character       Character to append.
    /// @param [in] numberOfChars   Number of times specifed character should be appended.
    ///
    void appendChar(char character, uint8_t numberOfChars)
    {
        if ((stringSize + numberOfChars) >= (STRING_BUFFER_SIZE-1))
            return; //overflow

        for (int i = 0; i < numberOfChars; i++)
            buffer[stringSize + i] = character;

        stringSize += numberOfChars;
        buffer[stringSize] = '\0';
    }

    ///
    /// \brief Concatenates given text to current string.
    /// @param [in] text    Text to Concatenate.
    ///
    void merge(const char* text)
    {
        if ((stringSize + strlen(text)) >= (STRING_BUFFER_SIZE-1))
            return; //overflow

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
        stringSize = 0;
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

    ///
    /// \brief Returns size of string in buffer.
    ///
    uint8_t getSize()
    {
        return stringSize;
    }

    private:
    ///
    /// \brief Holds current size of string.
    ///
    uint8_t stringSize;

    ///
    /// \brief String buffer used by all string functions.
    ///
    char buffer[STRING_BUFFER_SIZE];
};


/// @}

#endif
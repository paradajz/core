/*
    Copyright 2017-2019 Igor Petrovic

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

#ifndef __CORE_GENERAL_STRING_BUILDER
#define __CORE_GENERAL_STRING_BUILDER

#include <inttypes.h>
#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>

namespace core
{
    ///
    /// Helper class used for easier string manipulation.
    ///
    template<size_t stringSize>
    class StringBuilder
    {
        public:
        StringBuilder() {}

        ///
        /// \brief Returns current string.
        /// \returns Current string.
        ///
        const char* string()
        {
            return buffer;
        }

        ///
        /// \brief Overwrites the existing string with the specified string.
        ///
        void overwrite(const char* text, ...)
        {
            va_list args;
            va_start(args, text);
            vsnprintf(buffer, sizeof(buffer), text, args);
            va_end(args);
        }

        ///
        /// \brief Appends specified string to the existing string.
        ///
        void append(const char* text, ...)
        {
            va_list args;
            va_start(args, text);
            vsnprintf(&buffer[strlen(buffer)], sizeof(buffer), text, args);
            va_end(args);
        }

        ///
        /// \brief Fills the existing string with spaces.
        /// @param[in] size Number of spaces to append.
        ///
        bool fillUntil(size_t size)
        {
            size_t strSize = strlen(buffer);

            if ((strSize + size) >= (bufferSize - 1))
                return false;    // overflow

            for (size_t i = 0; i < size; i++)
                buffer[strSize + i] = ' ';

            strSize += size;
            buffer[strSize] = '\0';

            return true;
        }

        private:
        ///
        /// \brief Holds the maximum string size.
        ///
        const size_t bufferSize = stringSize;

        ///
        /// \brief Internal string buffer.
        ///
        char buffer[stringSize];
    };
}    // namespace core

#endif
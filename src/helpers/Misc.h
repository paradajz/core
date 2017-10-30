/*

Copyright 2017 Igor Petrović

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#pragma once

///
/// \brief Miscellaneous helpers.
/// \defgroup coreMiscHelpers Miscellaneous
/// \ingroup coreHelpers
/// @{

///
/// \brief Calculates size of array.
/// @param [in] x   Array for which size is being calculated.
/// \returns Array size in bytes.
///
#define ARRAY_SIZE(x)                   (sizeof((x)) / sizeof((x)[0]))

///
/// \brief Calculates size of char array.
/// @param [in] x   Array for which size is being calculated.
/// \returns Array size in bytes.
///
#define ARRAY_SIZE_CHAR(x)              (sizeof(x) / sizeof(x[0]) -1)

///
/// \brief Constrains input value to defined range.
/// @param [in] input   Input value.
/// @param [in] low     Lowest possible value.
/// @param [in] high    Largest possible value.
///
#define constrain(input, low, high)     ((input)<(low)?(low):((input)>(high)?(high):(input)))

///
/// \brief Checks number of digits for requested value.
/// @param[in] number   Number for which number if digits is being calculated.
/// \returns Number of digits.
///
inline uint8_t getNumberOfDigits(int32_t number)
{
    //make sure negative numbers are processed correctly
    number = abs(number);

    if (number < 10)
        return 1;

    if (number < 100)
        return 2;

    if (number < 1000)
        return 3;

    if (number < 10000)
        return 4;

    if (number < 1000000)
        return 6;

    if (number < 10000000)
        return 7;

    if (number < 100000000)
        return 8;

    if (number < 1000000000)
        return 9;

    return 10; //max size
}

/// @}
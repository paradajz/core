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

#include <stdio.h>
#include <stdlib.h>
#include <cstddef>
#include "core/src/ErrorHandler.h"

extern "C" void cxa_pure_virtual()
{
    core::errorHandler();
}

void* operator new(std::size_t size)
{
    return malloc(size);
}

void operator delete(void* ptr)
{
    free(ptr);
}

void operator delete(void* ptr, unsigned int size)
{
    free(ptr);
}

namespace std
{
    void __throw_bad_function_call()
    {
        core::errorHandler();

        while (1)
        {
            ;
        }
    }

    void __throw_bad_alloc()
    {
        core::errorHandler();

        while (1)
        {
            ;
        }
    }

    void __throw_out_of_range(char const* __s)
    {
        core::errorHandler();

        while (1)
        {
            ;
        }
    }

    void __throw_length_error(char const* __s)
    {
        core::errorHandler();

        while (1)
        {
            ;
        }
    }
}    // namespace std
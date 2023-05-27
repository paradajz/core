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
#include <optional>

namespace core::util
{
    template<typename T, uint32_t percentage>
    class EMAFilter
    {
        // exponential moving average filter
        public:
        EMAFilter() = default;

        T value(T raw)
        {
            _currentValue = (PERCENTAGE * static_cast<uint32_t>(raw) + (100 - PERCENTAGE) * static_cast<uint32_t>(_currentValue)) / 100;
            return _currentValue;
        }

        void reset()
        {
            _currentValue = 0;
        }

        private:
        static constexpr uint32_t PERCENTAGE    = percentage;
        T                         _currentValue = 0;
    };

    template<typename T, size_t size>
    class MedianFilter
    {
        static_assert(size % 2 != 0, "Median filter size must be odd.");

        public:
        MedianFilter() = default;

        std::optional<T> value(T raw)
        {
            auto compare = [](const void* a, const void* b)
            {
                if (*(T*)a < *(T*)b)
                {
                    return -1;
                }

                if (*(T*)a > *(T*)b)
                {
                    return 1;
                }

                return 0;
            };

            _analogSample[_medianSampleCounter++] = raw;

            // take the median value to avoid using outliers
            if (_medianSampleCounter == MEDIAN_SAMPLE_COUNT)
            {
                qsort(_analogSample, MEDIAN_SAMPLE_COUNT, sizeof(T), compare);
                _medianSampleCounter = 0;
                return _analogSample[MEDIAN_MIDDLE_VALUE];
            }
            else
            {
                return std::nullopt;
            }
        }

        void reset()
        {
            _medianSampleCounter = 0;
        }

        private:
        static constexpr size_t MEDIAN_SAMPLE_COUNT                = size;
        static constexpr size_t MEDIAN_MIDDLE_VALUE                = ((size + 1) / 2) - 1;
        T                       _analogSample[MEDIAN_SAMPLE_COUNT] = {};
        size_t                  _medianSampleCounter               = 0;
    };    // namespace IO
}    // namespace core::util

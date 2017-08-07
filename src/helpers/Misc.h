#pragma once

///
/// \brief Miscellaneous helper macros
/// \defgroup coreMiscHelpers Miscellaneous
/// \ingroup coreHelpers
/// @{

///
/// \brief Calculates size of array.
/// @param [in] x Array for which size is being calculated.
/// \returns Array size in bytes.
///
#define ARRAY_SIZE(x)                   (sizeof((x)) / sizeof((x)[0]))

///
/// \brief Calculates size of char array.
/// @param [in] x Array for which size is being calculated.
/// \returns Array size in bytes.
///
#define ARRAY_SIZE_CHAR(x)              (sizeof(x) / sizeof(x[0]) -1)

///
/// \brief Constrains input value to defined range.
/// @param [in] input Input value.
/// @param [in] low Lowest possible value.
/// @param [in] high Largest possible value.
///
#define constrain(input, low, high)     ((input)<(low)?(low):((input)>(high)?(high):(input)))

/// @}
#pragma once

///
/// \brief Size of stringBuffer and tempBuffer in bytes.
///
#define MAX_TEXT_SIZE   50

///
/// \brief String buffer used by all string functions. Must be declared externally.
///
extern char stringBuffer[MAX_TEXT_SIZE+1];

///
/// \brief Temporary buffer. Must be declared externally.
///
extern char tempBuffer[MAX_TEXT_SIZE+1];
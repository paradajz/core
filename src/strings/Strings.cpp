#include "Strings.h"

void addNumberToCharArray(int32_t number, uint8_t &stringSize)
{
    char intToCharArray[7];
    itoa(number, intToCharArray, 10);
    stringSize += strlen(intToCharArray);
    if (number < 0) stringSize++;
    strcat(stringBuffer, intToCharArray);
    stringBuffer[stringSize] = '\0';
}

void addByteToCharArray(uint8_t byte, uint8_t &stringSize)
{
    stringBuffer[stringSize] = byte;

    stringSize++;
    stringBuffer[stringSize] = '\0';
}

void appendText(const char *text, uint8_t &stringSize)
{
    stringSize += strlen(text);
    strcat(stringBuffer, text);
    stringBuffer[stringSize] = '\0';
}

void addSpaceToCharArray(uint8_t numberOfSpaces, uint8_t &stringSize)
{
    for (int i=0; i<numberOfSpaces; i++)
        stringBuffer[stringSize+i] = ' ';

    stringSize += numberOfSpaces;
        stringBuffer[stringSize] = '\0';
}

void startLine()
{
    stringBuffer[0] = '\0';
}

void endLine(uint8_t &stringSize)
{
    stringBuffer[stringSize] = '\0';
}
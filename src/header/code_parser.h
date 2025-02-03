#ifndef CODE_PARSER_H
#define CODE_PARSER_H

#include <Arduino.h>

namespace code_parser{
    enum CODE
    {
        START,
        STOP,
        DATA,
        NULL_CODE
    };
    
    CODE stringToCode(const String& str);
    extern CODE currentCode;

}

#endif // CODE_PARSER_H
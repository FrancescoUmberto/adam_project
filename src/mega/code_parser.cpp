#include <Arduino.h>
#include "header/code_parser.h"

namespace code_parser{

    CODE stringToCode(const String& str) {
        if (str.equals("START")) return START;
        if (str.equals("STOP")) return STOP;
        if (str.equals("DATA")) return DATA;
        return NULL_CODE;
    }

    CODE currentCode = NULL_CODE;

}
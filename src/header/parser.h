#ifndef PARSER_H
#define PARSER_H

#include <Arduino.h>


namespace parser{
    extern float duration;
    extern float sampling_period;
    extern String params;

    void parse_command(String &optional);
}

#endif

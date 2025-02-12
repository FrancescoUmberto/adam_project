#include <Arduino.h>
#include "header/utils.h"

namespace utils{
    bool substring(String& original, String& header, String& body) {
    int separatorIndex = original.indexOf(',');

    if ( original.indexOf(',') == -1) {
        header = original;
        body = "";
        return false;  
    }

    header = original.substring(0, separatorIndex);               
    body = original.substring(separatorIndex + 1);                 
    return true;
}
}
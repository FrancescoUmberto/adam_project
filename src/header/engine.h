#ifndef ENGINE_H
#define ENGINE_H

#include <Arduino.h>
#include <Servo.h>


namespace engine{
    extern Servo esc;
    void controlEngine(long startTime);
}



#endif
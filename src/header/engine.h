#ifndef ENGINE_H
#define ENGINE_H

#include <Arduino.h>
#include <Servo.h>

extern Servo esc;

namespace engine{

    void controlEngine();
}



#endif
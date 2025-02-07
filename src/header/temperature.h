#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <Arduino.h>
#include <SPI.h>

namespace temperature
{

    void processTemperatureSample();
    void begin();

}

#endif
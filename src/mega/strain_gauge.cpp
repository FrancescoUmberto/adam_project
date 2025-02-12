#include <Arduino.h>
#include <SPI.h>
#include <header/pin.h>
#include <header/data.h>

using namespace pin;
using namespace send_data;

namespace strain_gauge
{
    void processStrainSample()
    {
        if (Serial1.available() > 0) {
            String msg = Serial1.readStringUntil('\n');     
            globalData.setThrust((-1)*msg.toFloat());
            
        }
    }

}
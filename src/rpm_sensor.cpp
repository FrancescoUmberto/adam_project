#include <Arduino.h>
#include <header/rpm_sensor.h>
#include <header/pin.h>
#include <header/data.h>

using namespace send_data;

int rpmPin = 8;
bool rpmLastRead = false;

unsigned long timeLastRead = 0;
unsigned long lastTime = 0;
bool rpmRead;
unsigned long timeRead;
unsigned long deltaTimeRead;

namespace rpm
{
    void processRPMSample()
    {

        // RPM reading
        rpmRead = digitalRead(RMP_PIN);

        if (rpmRead && !rpmLastRead)
        {
            timeRead = micros();

            deltaTimeRead = timeRead - timeLastRead;
            timeLastRead = timeRead;
            
            globalData.setRPM(60000000 / deltaTimeRead);
        }

        rpmLastRead = rpmRead;
    }
}
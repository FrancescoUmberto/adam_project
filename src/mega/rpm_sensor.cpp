#include <Arduino.h>
#include <header/rpm_sensor.h>
#include <header/pin.h>
#include <header/data.h>

using namespace send_data;

int rpmPin = 8;
bool rpmLastRead = false;

unsigned long timeLastRead = 0;
unsigned long lastTime = 0;
unsigned long timeRead;
unsigned long deltaTimeRead;

// Fault tolerance for rising edge detection
const int DEBOUNCE_THRESHOLD = 2;
int zeroCount = 0;
int oneCount = 0;

// Exponential Moving Average (EMA) filter
float filteredRPM = 0.0;
const float EMA_ALPHA = 0.3; // Adjust for desired smoothness (0.1 - slow, 0.5 - fast)

namespace rpm
{
    void processRPMSample()
    {
        bool rpmRead = digitalRead(RMP_PIN);

        // Fault-tolerant rising edge detection
        if (rpmRead) {
            oneCount++;
            zeroCount = 0;  // Reset zero count when detecting 1

            if (oneCount >= DEBOUNCE_THRESHOLD && !rpmLastRead) {
                timeRead = micros();
                deltaTimeRead = timeRead - timeLastRead;
                timeLastRead = timeRead;

                // Compute raw RPM
                float rawRPM = 60000000.0 / deltaTimeRead;

                // Apply EMA filtering
                filteredRPM = (EMA_ALPHA * rawRPM) + ((1 - EMA_ALPHA) * filteredRPM);

                globalData.setRPM(filteredRPM);
            }
        } else {
            zeroCount++;
            oneCount = 0;  // Reset one count when detecting 0
        }

        // Ensure we have a valid edge detection
        if (zeroCount >= DEBOUNCE_THRESHOLD) {
            rpmLastRead = false;
        } else if (oneCount >= DEBOUNCE_THRESHOLD) {
            rpmLastRead = true;
        }
    }
}

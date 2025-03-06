#include <Arduino.h>
#include <header/rpm_sensor.h>
#include <header/pin.h>
#include <header/data.h>

using namespace send_data;

int rpmPin = 8;
bool rpmLastRead = false;

unsigned long timeLastRead = 0;
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
    // Flag to check if this is the first edge
    bool firstEdge = true;

    void setupRPM()
    {
        pinMode(RMP_PIN, INPUT);

        // Reset all variables
        timeLastRead = 0;
        filteredRPM = 0.0;
        firstEdge = true;

        digitalRead(RMP_PIN);
    }

    void processRPMSample()
    {
        bool rpmRead = digitalRead(RMP_PIN);

        // Fault-tolerant rising edge detection
        if (rpmRead) {
            oneCount++;
            zeroCount = 0;  

            if (oneCount >= DEBOUNCE_THRESHOLD && !rpmLastRead) {
                timeRead = micros();

                if (firstEdge) {
                    firstEdge = false;
                } else {
                    deltaTimeRead = timeRead - timeLastRead;
                    // Compute raw RPM; avoid division by zero if deltaTimeRead is unexpectedly 0
                    if (deltaTimeRead > 0) {
                        float rawRPM = 60000000.0 / deltaTimeRead;
                        // Apply EMA filtering
                        filteredRPM = (EMA_ALPHA * rawRPM) + ((1 - EMA_ALPHA) * filteredRPM);
                        globalData.setRPM(filteredRPM);
                    }
                }
                timeLastRead = timeRead;
            }
        } else {
            zeroCount++;
            oneCount = 0;
        }

        if (zeroCount >= DEBOUNCE_THRESHOLD) {
            rpmLastRead = false;
        } else if (oneCount >= DEBOUNCE_THRESHOLD) {
            rpmLastRead = true;
        }
    }
}

#include <Arduino.h>
#include <header/parser.h>
#include <header/pin.h>
#include <header/data.h>

unsigned int sample;
unsigned long startCalibration = 0;
double y;

using namespace parser;
using namespace pin;
using namespace send_data;

namespace microphone {

    void processAudioSample();

    void processAudioSample() {
        static unsigned long startMillis = 0;
        static unsigned int signalMax = 0;
        static unsigned int signalMin = 1024;
        static bool sampling = false;
        static float ema = 0;
        const float alpha = 0.1;
        static bool firstRun = true;

        unsigned long currentMillis = millis();

        if (!sampling) {
            startMillis = currentMillis;
            signalMax = 0;
            signalMin = 1024;
            sampling = true;
        }

        if (currentMillis - startMillis < sampling_period) {
            int sample = analogRead(MICROPHONE_PIN);  
            if (sample < 1024) {  // Discard anomalous values
                if (sample > signalMax) signalMax = sample;
                if (sample < signalMin) signalMin = sample;
            }
        } else {
            float peakToPeak = signalMax - signalMin;
            if (peakToPeak != 0) {
                float y = 55.5 + (1.2 * 10 * log(peakToPeak));
                
                if (firstRun) {
                    ema = y;
                    firstRun = false;
                } else {
                    ema = alpha * y + (1 - alpha) * ema;
                }
                globalData.setNoise(ema);
            }

            sampling = false;
        }
    }
}

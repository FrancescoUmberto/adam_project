#include <Arduino.h>
#include <header/parser.h>
#include <header/pin.h>
#include <header/data.h>

// Sample window width in mS (50 mS = 20Hz)
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
        static float ema = 0;          // EMA value for smoothing
        const float alpha = 0.1;       // Smoothing factor (0 < alpha <= 1)
        static bool firstRun = true;   // Flag to initialize the EMA

        unsigned long currentMillis = millis();

        if (!sampling) {
            // Initialize sampling period
            startMillis = currentMillis;
            signalMax = 0;
            signalMin = 1024;
            sampling = true;
        }

        // Sampling in progress
        if (currentMillis - startMillis < sampling_period) {
            int sample = analogRead(MICROPHONE_PIN);  
            if (sample < 1024) {  // Discard anomalous values
                if (sample > signalMax) signalMax = sample;
                if (sample < signalMin) signalMin = sample;
            }
        } else {
            // End of sampling period
            float peakToPeak = signalMax - signalMin;
            if (peakToPeak != 0) {
                float y = 55.5 + (1.2 * 10 * log(peakToPeak));
                
                // Apply exponential moving average smoothing
                if (firstRun) {
                    ema = y;
                    firstRun = false;
                } else {
                    ema = alpha * y + (1 - alpha) * ema;
                }
                globalData.setNoise(ema);
            }

            // Reset for the next cycle
            sampling = false;
        }
    }
}

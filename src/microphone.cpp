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

namespace microphone
{
    void processAudioSample();


void processAudioSample() {
    static unsigned long startMillis = 0;
    static unsigned int signalMax = 0;
    static unsigned int signalMin = 1024;
    static bool sampling = false;

    unsigned long currentMillis = millis();

    if (!sampling) {
        // Inizializza il campionamento
        startMillis = currentMillis;
        signalMax = 0;
        signalMin = 1024;
        sampling = true;
    }

    // Campionamento in corso
    if (currentMillis - startMillis < sampling_period) {
        int sample = analogRead(MICROPHONE_PIN);  
        if (sample < 1024) {  // Scarta valori anomali
            if (sample > signalMax) signalMax = sample;
            if (sample < signalMin) signalMin = sample;
        }
    } else {
        // Fine del periodo di campionamento
        float peakToPeak = signalMax - signalMin;
        if (peakToPeak != 0) {
            float y = 55.5 + (1.2 * 10 * log(peakToPeak));
            globalData.setNoise(y);
        }

        // Reset per il prossimo ciclo
        sampling = false;
    }
}

}
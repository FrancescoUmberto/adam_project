#include <Arduino.h>
#include <header/pin.h>
#include <header/data.h>
#include <header/voltage.h>
#include <header/parser.h>





using namespace parser;
using namespace pin;
using namespace send_data;

namespace voltage {
  const unsigned long sampleInterval = 3; // Time between samples in milliseconds
  const unsigned int numSamples = 500; // Number of samples to take for averaging
  unsigned long lastSampleTime = 0; // Time of the last sample
  unsigned int sampleCount = 0; // Number of samples taken so far
  float sampleSum = 0.0;  
  unsigned long currentTime;

void processVoltageSample() {
  currentTime = millis();

  if (sampleCount < numSamples) {
    if (currentTime - lastSampleTime >= sampleInterval) {
      lastSampleTime = currentTime;
      int reading = analogRead(VOLTAGE_PIN);
      sampleSum += reading;
      sampleCount++;

    }
  } 
  else {
    float avgVcs = sampleSum / numSamples; 
    float voltage = (7*avgVcs/133) + 0.2632;
    
    globalData.setVoltage(voltage);
    
    sampleCount = 0;
    sampleSum = 0.0;
  }
}

}
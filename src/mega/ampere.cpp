#include <Arduino.h>
#include <header/pin.h>
#include <header/data.h>
#include <header/ampere.h>
#include <header/parser.h>





using namespace parser;
using namespace pin;
using namespace send_data;

namespace ampere {
  const unsigned long sampleInterval = 3;
  const unsigned int numSamples = 100;
  unsigned long lastSampleTime = 0;
  unsigned int sampleCount = 0;
  float sampleSum = 0.0;  
  unsigned long currentTime;

void processAmpereSample() {
  currentTime = millis();
  
  if (sampleCount < numSamples) {
    if (currentTime - lastSampleTime >= sampleInterval) {
      lastSampleTime = currentTime;
      int reading = analogRead(AMPERE_PIN);
      sampleSum += reading;
      sampleCount++;

    }
  } 
  else {
    float avgAcs = sampleSum / numSamples;
    float current = -(avgAcs - 517.55) / 13.0;
    
    globalData.setCurrent(current);
    
    sampleCount = 0;
    sampleSum = 0.0;
  }
}

}
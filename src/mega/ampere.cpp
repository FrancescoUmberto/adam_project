#include <Arduino.h>
#include <header/pin.h>
#include <header/data.h>
#include <header/ampere.h>
#include <header/parser.h>





using namespace parser;
using namespace pin;
using namespace send_data;

namespace ampere {
  const unsigned long sampleInterval = 3;    // Intervallo tra i campioni in millisecondi
  const unsigned int numSamples = 100;        // Numero di campioni da acquisire
  unsigned long lastSampleTime = 0;            // Tempo dell'ultimo campione
  unsigned int sampleCount = 0;                // Contatore dei campioni acquisiti
  float sampleSum = 0.0;  
  unsigned long currentTime;                     // Somma dei campioni

void processAmpereSample() {
  currentTime = millis();
  
  // Se non sono stati ancora acquisiti tutti i campioni richiesti...
  if (sampleCount < numSamples) {
    // ...verifica che sia trascorso l'intervallo desiderato
    if (currentTime - lastSampleTime >= sampleInterval) {
      lastSampleTime = currentTime;
      int reading = analogRead(AMPERE_PIN);      // Legge il valore dal sensore
      sampleSum += reading;              // Accumula il campione
      sampleCount++;

    }
  } 
  // Una volta acquisiti tutti i campioni, calcola la media e la corrente
  else {
    float avgAcs = sampleSum / numSamples;  // Media dei campioni
    // Oppure, se preferisci utilizzare la formula fornita per il calcolo della corrente:
    float current = -(avgAcs - 517.55) / 13.0;
    
    // Stampa il risultato sul monitor seriale
    globalData.setCurrent(current);
    
    // Resetta le variabili per iniziare una nuova acquisizione
    sampleCount = 0;
    sampleSum = 0.0;
  }
}

}
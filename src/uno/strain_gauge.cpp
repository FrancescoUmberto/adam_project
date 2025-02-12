#include <HX711_ADC.h>

#define DATA_PIN 3
#define CLOCK_PIN 5

HX711_ADC LoadCell(DATA_PIN, CLOCK_PIN);
float f;
bool start = false;

void setup()
{
  Serial.begin(250000);
  LoadCell.begin();
  LoadCell.start(2000);           // Start HX711 and wait for it to stabilize (2 seconds)
  LoadCell.setCalFactor(1037.61); // Set calibration factor (adjust as needed)
}

void loop()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    if (command.indexOf("START") != -1)
    {
      start = true;
    }
    else if (command.indexOf("STOP") != -1)
      start = false;
  }

  if (start)
  {
    LoadCell.update();
    float weight = LoadCell.getData();
    Serial.println(weight, 2);
  }
}
#include <HX711.h>

#define DATA_PIN 3
#define CLOCK_PIN 5

HX711 LoadCell;
float f;
bool start = false;

void setup()
{
  Serial.begin(250000);
  LoadCell.begin(DATA_PIN, CLOCK_PIN);

  LoadCell.set_scale(1035.951538);       // TODO you need to calibrate this yourself.
  LoadCell.set_offset(103738.25);
  
  // LoadCell.set_gain(1037.61); // Set calibration factor (adjust as needed)
}

void loop()
{
  if (Serial.available() > 0)
  {
    String command = Serial.readStringUntil('\n');
    if (command.indexOf("START") != -1)
    {
      start = true;
      LoadCell.tare();
    }
    else if (command.indexOf("STOP") != -1)
      start = false;
  }

  if (start)
  {
    f = LoadCell.get_units(2);
    Serial.println(f, 2);
  }
}
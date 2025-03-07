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

  LoadCell.set_scale(1035.951538);       
  LoadCell.set_offset(103738.25);
  
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
      LoadCell.begin(DATA_PIN, CLOCK_PIN);
      LoadCell.set_scale(1035.951538);       
      LoadCell.set_offset(103738.25);
  }

  if (start)
  {
    f = LoadCell.get_units(2);
    Serial.println(f, 2);
  }
}
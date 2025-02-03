#include <Arduino.h>
#include <header/parser.h>
#include <header/mode.h>
#include <header/code_parser.h>

using namespace mode;
using namespace code_parser;


void start(String& optional);
bool substring(String& original, String& header, String& body);

String curve_in;

enum CODE code;
String code_in;

String params;

String mode_in;

float duration;
float sampling_period;

String inputString;


String temp_string;

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Inserire il codice di comando: ");

}

void loop()
{

  if (Serial.available())
  {
    inputString = Serial.readStringUntil('\n');
    Serial.print("Hai inserito: ");
    Serial.println(inputString);

    if (substring(inputString, code_in, params))
    {
      code = stringToCode(code_in);
      Serial.print("Code: ");
      Serial.println(code_in);

      if (code == CODE::START)
      {
        start(params);
      }
      else if (code == CODE::STOP)
      {
        return;
      }
    }
    else
    {
      Serial.println("Parsing Error");
    }
  }
}

void start(String& optional)
{
  if (substring(optional, mode_in, params))
  {
    // define MODE
    mode::currentMode = stringToMode(mode_in);
    Serial.print("Mode: ");
    Serial.println(mode_in);

    // define DURATION
    substring(params, temp_string, params);
    duration = temp_string.toFloat();
    Serial.print("Duration: ");
    Serial.println(duration);

    // define SAMPLING_PERIOD
    substring(params, temp_string, params);
    sampling_period = temp_string.toFloat();
    Serial.print("Sampling Period: ");
    Serial.println(sampling_period);

    if (mode::currentMode == MODE::SINGLE)
    {
      globalSingleSpeedMode.setDutyCycle(params.toFloat());
      Serial.print("Duty Cycle: ");
      Serial.println(globalSingleSpeedMode.getDutyCycle());

      globalSingleSpeedMode.getParams();
    }
    else if (mode::currentMode == MODE::SWEEP)
    {
      // define INITIAL_DC
      substring(params, temp_string, params);
      globalSweepMode.setInitialDC(temp_string.toInt());
      Serial.print("Initial Duty Cycle: ");
      Serial.println(globalSweepMode.getInitialDC());

      // define FINAL_DC
      substring(params, temp_string, params);
      globalSweepMode.setFinalDC(temp_string.toInt());
      Serial.print("Final Duty Cycle: ");
      Serial.println(globalSweepMode.getFinalDC());

      // define CURVE
      substring(params, temp_string, params);
      globalSweepMode.setCurve(temp_string);
      Serial.print("Curve: ");
      Serial.println(temp_string);

      if(globalSweepMode.getCurve() == CURVE::STEPS){
        // define N_STEPS
        globalSweepMode.setNSteps(params.toInt());
        Serial.print("Number of Steps: ");
        Serial.println(globalSweepMode.getNSteps());
      }


      globalSweepMode.getParams();
    }
    else if (mode::currentMode == MODE::SETPOINT)
    {
      substring(params, temp_string, params);
      globalSetPointMode.setSetPoint(temp_string);
      Serial.print("Setpoint: ");
      Serial.println(temp_string);

      globalSetPointMode.setValue(params.toFloat());
      Serial.print("Value: ");
      Serial.println(globalSetPointMode.getValue());

      globalSetPointMode.getParams();
    }
  }
  else
  {
    Serial.println("Parsing Error");
  }
}

bool substring(String& original, String& header, String& body) {
    int separatorIndex = original.indexOf(';');

    if ( original.indexOf(';') == -1) {
        header = original;
        body = "";
        return false;  
    }

    header = original.substring(0, separatorIndex);               
    body = original.substring(separatorIndex + 1);                 
    return true;
}

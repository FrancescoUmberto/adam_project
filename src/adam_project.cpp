#include <Arduino.h>
#include <header/parser.h>
#include <header/mode.h>

using namespace mode;


void start(char *optional);
bool substring(char *original, char *header, char *body);

enum CODE
{
  START,
  STOP,
  DATA,
  NULL_CODE
};

CODE stringToCode(const char *str)
{
  if (strcmp(str, "START") == 0)
    return START;
  if (strcmp(str, "STOP") == 0)
    return STOP;
  if (strcmp(str, "DATA") == 0)
    return DATA;
  return NULL_CODE;
}


char curve_in[10];

enum CODE code;
char code_in[5];

char params[90];

char mode_in[10];

float duration;
float sampling_period;


char temp_string[10];

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Inserire il codice di comando: ");

}

void loop()
{

  char inputString[100];
  if (Serial.available())
  {
    int len = Serial.readBytesUntil('\n', inputString, sizeof(inputString) - 1);
    inputString[len] = '\0';
    Serial.print("Hai inserito: ");
    Serial.println(inputString);

    if (substring(inputString, code_in, params))
    {
      code = stringToCode(code_in);
      Serial.print("Code: ");
      Serial.println(code_in);
      Serial.print("params: ");
      Serial.println(params);
      if (code == CODE::START)
      {
        start(params);
      }
      else if (code == CODE::STOP)
      {
        // handle the stop command (stop everything)
        return;
      }
    }
    else
    {
      Serial.println("Parsing Error");
    }
  }
}

void start(char *optional)
{
  Serial.print("Code: START ");
  Serial.println(optional);
  if (substring(optional, mode_in, params))
  {
#define MODE
    mode::currentMode = stringToMode(mode_in);
    Serial.print("Mode: ");
    Serial.println(mode_in);

#define DURATION
    substring(params, temp_string, params);
    duration = atof(temp_string);
    Serial.print("Duration: ");
    Serial.println(duration);

#define SAMPLING_PERIOD
    substring(params, temp_string, params);
    sampling_period = atof(temp_string);
    Serial.print("Sampling Period: ");
    Serial.println(sampling_period);

    if (mode::currentMode == MODE::SINGLE)
    {
      substring(params, temp_string, params);
      globalSingleSpeedMode.setDutyCycle(atof(temp_string));
      Serial.print("Duty Cycle: ");
      Serial.println(globalSingleSpeedMode.getDutyCycle());
    }
    else if (mode::currentMode == MODE::SWEEP)
    {
#define INITIAL_DC
      substring(params, temp_string, params);
      globalSweepMode.setInitialDC(atoi(temp_string));
      Serial.print("Initial Duty Cycle: ");
      Serial.println(globalSweepMode.getInitialDC());

#define FINAL_DC
      substring(params, temp_string, params);
      globalSweepMode.setFinalDC(atoi(temp_string));
      Serial.print("Final Duty Cycle: ");
      Serial.println(globalSweepMode.getFinalDC());

#define CURVE
      substring(params, curve_in, params);
      globalSweepMode.setCurve(curve_in);
      Serial.print("Curve: ");
      Serial.println(curve_in);
    }
    else if (mode::currentMode == MODE::SETPOINT)
    {
      substring(params, temp_string, params);
      globalSetPointMode.setSetPoint(temp_string);
      Serial.print("Setpoint: ");
      Serial.println(temp_string);

      substring(params, temp_string, params);
      globalSetPointMode.setValue(atof(temp_string));
      Serial.print("Value: ");
      Serial.println(globalSetPointMode.getValue());
    }
  }
  else
  {
    Serial.println("Parsing Error");
  }
}

bool substring(char *original, char *header, char *body)
{
  if (sscanf(original, "%[^;];%[^.*]", header, body))
    return true;
  return false;
}

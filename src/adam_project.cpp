#include <Arduino.h>

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

enum MODE
{
  SINGLE,
  SWEEP,
  SETPOINT,
  NULL_MODE
};

MODE stringToMode(const char *str)
{
  if (strcmp(str, "SINGLE") == 0)
    return SINGLE;
  if (strcmp(str, "SWEEP") == 0)
    return SWEEP;
  if (strcmp(str, "SETPOINT") == 0)
    return SETPOINT;
  return NULL_MODE;
}

enum SETPOINTTYPE
{
  DUTY,
  RPM,
  POWER,
  THRUST,
  NULL_SETPOINT
};

SETPOINTTYPE stringToSetPoint(const char *str)
{
  if (strcmp(str, "DUTY") == 0)
    return DUTY;
  if (strcmp(str, "RPM") == 0)
    return RPM;
  if (strcmp(str, "POWER") == 0)
    return POWER;
  if (strcmp(str, "THRUST") == 0)
    return THRUST;
  return NULL_SETPOINT;
}

enum CURVE
{
  RAMP,
  STEPS,
  GENERIC,
  NULL_CURVE
};

char curve_in[10];

CURVE stringToCurve(const char *str)
{
  if (strcmp(str, "RAMP") == 0)
    return RAMP;
  if (strcmp(str, "STEPS") == 0)
    return STEPS;
  if (strcmp(str, "GENERIC") == 0)
    return GENERIC;
  return NULL_CURVE;
}

enum CODE code;
char code_in[5];

char params[90];

enum MODE mode;
char mode_in[10];

float duration;
float sampling_period;

struct SingleSpeedMode
{
  float duty_cycle;
};

SingleSpeedMode single_speed_mode;

struct SetPointMode
{
  enum SETPOINTTYPE setpoint;
  float value;
};

SetPointMode set_point_mode;

struct SweepMode
{
  int initial_dc;
  int final_dc;
  enum CURVE curve;
};

SweepMode sweep_speed_mode;

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
    mode = stringToMode(mode_in);
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

    if (mode == MODE::SINGLE)
    {
      substring(params, temp_string, params);
      single_speed_mode.duty_cycle = atof(temp_string);
      Serial.print("Duty Cycle: ");
      Serial.println(single_speed_mode.duty_cycle);
    }
    else if (mode == MODE::SWEEP)
    {
#define INITIAL_DC
      substring(params, temp_string, params);
      sweep_speed_mode.initial_dc = atoi(temp_string);
      Serial.print("Initial Duty Cycle: ");
      Serial.println(sweep_speed_mode.initial_dc);

#define FINAL_DC
      substring(params, temp_string, params);
      sweep_speed_mode.final_dc = atoi(temp_string);
      Serial.print("Final Duty Cycle: ");
      Serial.println(sweep_speed_mode.final_dc);

#define CURVE
      substring(params, curve_in, params);
      sweep_speed_mode.curve = stringToCurve(curve_in);
      Serial.print("Curve: ");
      Serial.println(curve_in);
    }
    else if (mode == MODE::SETPOINT)
    {
      substring(params, temp_string, params);
      set_point_mode.setpoint = stringToSetPoint(temp_string);
      Serial.print("Setpoint: ");
      Serial.println(temp_string);

      substring(params, temp_string, params);
      set_point_mode.value = atof(temp_string);
      Serial.print("Value: ");
      Serial.println(set_point_mode.value);
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

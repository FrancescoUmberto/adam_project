#include <Arduino.h>

void start(String optional);

enum CODE
{
  START,
  STOP,
  DATA
};

enum MODE
{
  SINGLE,
  SWEEP,
  SETPOINT
};

enum SETPOINT
{
  DUTY,
  RPM,
  POWER,
  THRUST
};

enum CURVE
{
  RAMP,
  STEPS,
  GENERIC
};

enum CODE code;
char code_in[5];

char optional[100];


enum MODE mode;
char mode_in[10];

float duration;
float sampling_period;

struct single_speed_mode
{
  float duty_cycle;
};

struct sweep_speed_mode
{
  enum SETPOINT setpoint;
  float value;
};

struct sweep_mode
{
  int initial_dc;
  int final_dc;
  enum CURVE curve;
};

char am_setpoint[1];
float am_value;

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
    inputString[len] = '\0'; // Aggiunge il terminatore di stringa
    Serial.print("Hai inserito: ");
    Serial.println(inputString);
    if (sscanf(inputString, "%[^;];", code_in))
    {
      code = (enum CODE)atoi(code_in);
      if(code == CODE::START){
        start("C");
      }
    }
    else
    {
      Serial.println("Errore di parsing");
    }
  }
}


void start(String optional){
  Serial.println(optional);
}

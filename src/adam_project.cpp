#include <Arduino.h>
#include <header/parser.h>
#include <header/mode.h>
#include <header/code_parser.h>
#include <header/utils.h>

using namespace parser;
using namespace mode;
using namespace code_parser;
using namespace utils;

String curve_in;

enum CODE code;
String code_in;
String inputString;
bool flag;

void setup()
{
  cli();  // Disabilita gli interrupt globali

  Serial.begin(250000);
  Serial.setTimeout(100);
  Serial.println("Inserire il codice di comando: ");

  TCCR1A = 0;
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  
  // OCR1A = (16e6 / (1024 * desiredFrequency)) - 1
  OCR1A = (16000000UL / (1024UL)) - 1;
  
  TIMSK1 = (1 << OCIE1A);

  sei(); 

}

ISR(TIMER1_COMPA_vect) {
  flag = true;
}


void loop()
{
  if(flag){
    globalSingleSpeedMode.getParams();
    flag = false;
  }

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
        cli();
        parse_command(params);
        sei();
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

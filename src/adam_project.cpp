#include <Arduino.h>
#include <header/parser.h>
#include <header/mode.h>
#include <header/code_parser.h>
#include <header/utils.h>
#include <header/data.h>

using namespace parser;
using namespace mode;
using namespace code_parser;
using namespace utils;
using namespace send_data;

String curve_in;
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
  if(flag && currentCode == CODE::START){
    globalData.sendData();
    flag = false;
  }

  if (Serial.available())
  {
    inputString = Serial.readStringUntil('\n');
    Serial.print("Hai inserito: ");
    Serial.println(inputString);

    if (substring(inputString, code_in, params))
    {
      currentCode = stringToCode(code_in);
      if (currentCode == CODE::START)
      {
        cli();
        parse_command(params);
        sei();
      }
      else if (currentCode == CODE::STOP)
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

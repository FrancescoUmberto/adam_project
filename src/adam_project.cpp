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
        parse_command(params);
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

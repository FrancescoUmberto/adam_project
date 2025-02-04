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

  Serial.begin(230400);
  Serial.println("Inserire il codice di comando per impostare la frequenza (Hz): ");

  // Configurazione di Timer1 in modalità CTC
  TCCR1A = 0;
  // Modalità CTC e prescaler = 1024 (CS12=1 e CS10=1)
  TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  
  // Calcola OCR1A per la frequenza iniziale:
  // OCR1A = (16e6 / (1024 * desiredFrequency)) - 1
  OCR1A = (16000000UL / (1024UL)) - 1;
  
  // Abilita l'interrupt per il confronto A del Timer1
  TIMSK1 = (1 << OCIE1A);

  sei();  // Riabilita gli interrupt globali 

}

ISR(TIMER1_COMPA_vect) {
  flag = true;
}


void loop()
{
  if(flag){
    Serial.println("Manca tutto");
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

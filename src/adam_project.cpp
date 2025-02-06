#include <Arduino.h>
#include <header/parser.h>
#include <header/mode.h>
#include <header/code_parser.h>
#include <header/utils.h>
#include <header/data.h>
#include <header/engine.h>
#include <header/pin.h>
#include <header/microphone.h>

using namespace parser;
using namespace mode;
using namespace code_parser;
using namespace utils;
using namespace send_data;
using namespace engine;
using namespace pin;
using namespace microphone;

String curve_in;
String code_in;
String inputString;
long startTime;
bool startTimeCount = false;
bool flag;

void setup()
{
  esc.attach(ESC_PIN);         // Attach the ESC to the pin
  esc.writeMicroseconds(1000); // Initialize ESC with the current speed
  pinMode(RMP_PIN, INPUT);

  cli(); // Disabilita gli interrupt globali

  Serial.begin(250000);
  Serial.setTimeout(100);
  Serial.println("Inserire il codice di comando: ");

  TCCR3A = 0;
  TCCR3B = (1 << WGM12) | (1 << CS12) | (1 << CS10);

  // OCR1A = (16e6 / (1024 * desiredFrequency)) - 1
  OCR3A = (16000000UL / (1024UL)) - 1;

  TIMSK3 = (1 << OCIE1A);

  sei();
  pinMode(MICROPHONE_PIN, INPUT); // Set the signal pin as input
}

ISR(TIMER3_COMPA_vect)
{
  flag = true;
}

void loop()
{
  if (flag && currentCode == CODE::START && micros() - startTime <= duration * 1000)
  {
    globalData.sendData();
    flag = false;
  }
  if (currentCode == CODE::START && micros() - startTime > duration * 1000)
  {
    currentCode = CODE::STOP;
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
      // replace with a boolean value and take the time only when the code is START and the speed is reached
      startTimeCount = true;
      startTime = micros();
    }
    else
    {
      Serial.println("Parsing Error");
    }
  }

  if (currentCode == CODE::START)
  {
    processAudioSample();
  }
;
  globalSingleSpeedMode.getParams();

  controlEngine(startTime);
}

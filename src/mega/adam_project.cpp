#include <Arduino.h>
#include <SPI.h>
#include <header/parser.h>
#include <header/mode.h>
#include <header/code_parser.h>
#include <header/utils.h>
#include <header/data.h>
#include <header/engine.h>
#include <header/pin.h>
#include <header/microphone.h>
#include <header/rpm_sensor.h>
#include <header/temperature.h>
#include <header/strain_gauge.h>
#include <header/ampere.h>
#include <header/voltage.h>

using namespace parser;
using namespace mode;
using namespace code_parser;
using namespace utils;
using namespace send_data;
using namespace engine;
using namespace pin;
using namespace microphone;
using namespace rpm;
using namespace temperature;
using namespace strain_gauge;
using namespace ampere;
using namespace voltage;

String curve_in;
String code_in;
String inputString;
long startTime;
bool startTimeCount = false;
bool flag;

void setup()
{
  esc.attach(ESC_PIN);        
  esc.writeMicroseconds(1000); 
  pinMode(RMP_PIN, INPUT);

  cli(); 

  Serial.begin(250000);
  Serial1.begin(250000);
  Serial.setTimeout(100);

  TCCR5A = 0;
  TCCR5B = (1 << WGM12) | (1 << CS12) | (1 << CS10);

  OCR5A = (16000000UL / (1024UL)) - 1;

  TIMSK5 = (1 << OCIE1A);

  sei();
  pinMode(MICROPHONE_PIN, INPUT);
  pinMode(RMP_PIN, INPUT);
}

ISR(TIMER5_COMPA_vect)
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

  if (currentCode == CODE::START && micros() - startTime > duration * 1000 )
  {
    cli();
    esc.detach();
    Serial.println("STOP");
    Serial1.println("STOP");
    globalData.reset();
    currentCode = CODE::STOP;
    sei();
  }

  if (Serial.available())
  {
    inputString = Serial.readStringUntil('\n');

    if (substring(inputString, code_in, params))
    {
      currentCode = stringToCode(code_in);
      if (currentCode == CODE::START)
      {
        cli();   
        parse_command(params);
        sei();
        esc.attach(ESC_PIN);  
        delay(2000);
        esc.writeMicroseconds(1000);
        delay(1000);
        temperature::begin();

        rpm::setupRPM();
        
        Serial1.println("START");

        startTimeCount = true;
        startTime = micros();
      }
      else if (currentCode == CODE::STOP)
      {
        globalData.reset();
        esc.detach();
        return;
      }

    }
    else
    {
      Serial.println("Parsing Error");
    }
  }

  if (currentCode == CODE::START)
  {
    processAudioSample();
    processRPMSample();
    processTemperatureSample();
    processStrainSample();
    processAmpereSample();
    processVoltageSample();
  };

  controlEngine(startTime);
}

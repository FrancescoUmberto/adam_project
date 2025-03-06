#include <Arduino.h>
#include <header/engine.h>
#include <header/pin.h>
#include <Servo.h>
#include <header/mode.h>
#include <header/code_parser.h>
#include <header/parser.h>

using namespace code_parser;
using namespace parser;
using namespace mode;
using namespace pin;

namespace engine
{
  Servo esc;

  float interpolationStep = 0.0001;
  float currentSpeed = 1000;
  float currentSpeedSweep = 1000;
  unsigned long lastTime = 0;
  long elapsedTime = 0;

  long initial_dc;
  long final_dc;

  unsigned long time;
  float deltaTime;
  int readCount = 0;

  void accelerationHandling(long targetSpeed);
  void stopEngine();
  void singleModeHandling();
  void rampControl(long currentTime, long minSpeed, long maxSpeed);

  void controlEngine(long startTime)
  {
    if (currentCode == CODE::START)
    {
      if (currentMode == MODE::SINGLE)
      {
        singleModeHandling();
      }
      else if (currentMode == MODE::SWEEP)
      {
        if (globalSweepMode.getCurve() == CURVE::RAMP)
        {
          rampControl(startTime, globalSweepMode.getInitialDC(), globalSweepMode.getFinalDC());
        }
      }
      else if (currentMode == MODE::SETPOINT)
      {
      }
    }
    else if (currentCode == CODE::STOP)
    {

      stopEngine();
      currentSpeed = 1000;
      currentSpeedSweep = 1000;
      lastTime = 0;
      elapsedTime = 0;
      time = 0;
      deltaTime = 0;
    }
  }

  void singleModeHandling()
  {
    long targetSpeed = globalSingleSpeedMode.getTargetSpeed();
    accelerationHandling(targetSpeed);
  }

  void stopEngine()
  {
    //esc.writeMicroseconds(1000);
    //accelerationHandling(1000);
    
  }

  void accelerationHandling(long targetSpeed)
  {
    if (lastTime == 0)
    {
      lastTime = micros();
      return;
    }

    time = micros();
    deltaTime = time - lastTime;
    lastTime = time;

    if (currentSpeed < targetSpeed)
    {
      currentSpeed += interpolationStep * deltaTime;
      if (currentSpeed > targetSpeed)
      {
        currentSpeed = targetSpeed;
      }
    }
    else if (currentSpeed > targetSpeed)
    {
      currentSpeed -= interpolationStep * deltaTime;
      if (currentSpeed < targetSpeed)
      {
        currentSpeed = targetSpeed; 
      }
    }

    esc.writeMicroseconds((int)currentSpeed);
  }

  void rampControl(long startTime, long minSpeed, long maxSpeed)
  {
    time = micros();
    elapsedTime = time - startTime;

    if (elapsedTime / 1000 >= duration)
    {
      currentSpeedSweep = maxSpeed; 
      return;
    }
    currentSpeedSweep = minSpeed + (((elapsedTime / 1000) * (maxSpeed - minSpeed)) / duration);

    esc.writeMicroseconds((int)currentSpeedSweep);
  }

}

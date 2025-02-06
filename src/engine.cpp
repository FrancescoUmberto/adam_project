#include <Arduino.h>
#include <header/engine.h>
#include <header/pin.h>
#include <Servo.h>
#include <header/mode.h>
#include <header/code_parser.h>

using namespace code_parser;
using namespace mode;
using namespace pin;

namespace engine
{
  Servo esc;

  float targetSpeed = 1000;    // Target speed (in microseconds)
  float interpolationStep = 1; // Speed change per iteration (the smaller, the smoother)
  float currentSpeed = 1000;
  unsigned long lastTime = 0;

  // Internal
  unsigned long time;
  float deltaTime;
  int readCount = 0;

  void accelerationHandling(long targetSpeed);
  void stopEngine();
  void singleModeHandling();

  void controlEngine()
  {
    if (currentCode == CODE::START)
    {
      if (currentMode == MODE::SINGLE)
      {
        singleModeHandling();
      }
      else if (currentMode == MODE::SWEEP)
      {
        Serial.println("SWEEP MODE");
      }
      else if (currentMode == MODE::SETPOINT)
      {
        Serial.println("SETPOINT MODE");
      }
    }
    else if (currentCode == CODE::STOP)
    {
      stopEngine();
    }
  }

  void singleModeHandling()
  {
    long targetSpeed = globalSingleSpeedMode.getTargetSpeed();
    accelerationHandling(targetSpeed);

    
  }

  void stopEngine()
  {
    accelerationHandling(0);
  }

  void accelerationHandling(long targetSpeed)
  {
    time = micros();
    deltaTime = time - lastTime;
    lastTime = time;

    // Smoothly interpolate the speed towards the target speed
    if (currentSpeed < targetSpeed)
    {
      currentSpeed += interpolationStep / deltaTime; // Increase speed
      if (currentSpeed > targetSpeed)
      {
        currentSpeed = targetSpeed; // Cap at the target speed
      }
    }
    else if (currentSpeed > targetSpeed)
    {
      currentSpeed -= interpolationStep / deltaTime; // Decrease speed
      if (currentSpeed < targetSpeed)
      {
        currentSpeed = targetSpeed; // Cap at the target speed
      }
    }

    esc.writeMicroseconds((int)currentSpeed);
  }

}

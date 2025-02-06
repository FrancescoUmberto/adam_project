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

  float targetSpeed = 1000;    // Target speed (in microseconds)
  float interpolationStep = 0.0001; // Speed change per iteration (the smaller, the smoother)
  float currentSpeed = 1000;
  unsigned long lastTime = 0;
  long elapsedTime = 0;

  long initial_dc;
  long final_dc;

  // Internal
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
        rampControl(startTime, globalSweepMode.getInitialDC(), globalSweepMode.getFinalDC());
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
    accelerationHandling(1000);
  }

  void accelerationHandling(long targetSpeed)
  {
    if(lastTime == 0){
      lastTime = micros();
      return;
    }
    
    time = micros();
    deltaTime = time - lastTime;
    lastTime = time;

    // Smoothly interpolate the speed towards the target speed
    if (currentSpeed < targetSpeed)
    {
      currentSpeed += interpolationStep * deltaTime; // Increase speed
      if (currentSpeed > targetSpeed)
      {
        currentSpeed = targetSpeed; // Cap at the target speed
      }
    }
    else if (currentSpeed > targetSpeed)
    {
      currentSpeed -= interpolationStep * deltaTime; // Decrease speed
      if (currentSpeed < targetSpeed)
      {
        currentSpeed = targetSpeed; // Cap at the target speed
      }
    }

    esc.writeMicroseconds((int)currentSpeed);
  }

  void rampControl(long startTime, long minSpeed, long maxSpeed)
  {

    time = micros();
    elapsedTime = time - startTime;
    startTime = time; // Initialize start time
    if (elapsedTime >= duration)
    {
      currentSpeed = maxSpeed; // Ensure we reach the target speed at the end
      return;
    }

    // Linearly interpolate speed based on elapsed time
    currentSpeed = minSpeed + ((elapsedTime * (maxSpeed - minSpeed)) / duration);

    esc.writeMicroseconds((int)currentSpeed);
  }

}

#include <Arduino.h>
#include "header/mode.h"
#include "header/pin.h"

using namespace pin;
namespace mode {

    MODE stringToMode(const String& str)
    {
        if (str.equals("SINGLE")) return SINGLE;
        if (str.equals("SWEEP")) return SWEEP;
        if (str.equals("SETPOINT")) return SETPOINT;
        return NULL_MODE;
    }

    SETPOINTTYPE stringToSetPoint(const String& str)
    {
        if (str.equals("DUTY")) return DUTY;
        if (str.equals("RPM")) return RPM;
        if (str.equals("POWER")) return POWER;
        if (str.equals("THRUST")) return THRUST;
        return NULL_SETPOINT;
    }

    CURVE stringToCurve(const String& str)
    {
        if (str.equals("RAMP")) return RAMP;
        if (str.equals("STEPS")) return STEPS;
        return NULL_CURVE;
    }



    SweepMode::SweepMode()
        : initial_dc(0), final_dc(0), curve(NULL_CURVE)
    {
    }

    long SweepMode::getInitialDC()
    {
        return initial_dc;
    }

    void SweepMode::setInitialDC(int initial_dc)
    {
        if (initial_dc >= 0 && initial_dc <= 100)
        {
            this->initial_dc = map(initial_dc, 0, 100, MIN_SPEED, MAX_SPEED);
        }
        else
        {
            Serial.println("Invalid input! Enter a value between 0 and 100");
            this->initial_dc = 0;
        }
    }

    long SweepMode::getFinalDC()
    {
        return final_dc;
    }

    void SweepMode::setFinalDC(int final_dc)
    {
        if (final_dc >= 0 && final_dc <= 100)
        {
            this->final_dc = map(final_dc, 0, 100, MIN_SPEED, MAX_SPEED);
        }
        else
        {
            Serial.println("Invalid input! Enter a value between 0 and 100");
            this->final_dc = 0;
        }
    }

    int SweepMode::getNSteps()
    {
        return n_steps;
    }

    void SweepMode::setNSteps(int n_steps)
    {
        this->n_steps = n_steps;
    }

    CURVE SweepMode::getCurve()
    {
        return curve;
    }

    void SweepMode::setCurve(CURVE curve)
    {
        this->curve = curve;
    }

    void SweepMode::setCurve(const String& curveStr)
    {
        this->curve = stringToCurve(curveStr);
    }


    void SweepMode::getParams()
    {
        Serial.println("----------------------------");
        Serial.println("Sweep Mode Parameters: ");
        Serial.print("Initial Duty Cycle: ");
        Serial.println(initial_dc);
        Serial.print("Final Duty Cycle: ");
        Serial.println(final_dc);
        Serial.print("Curve: ");
        Serial.println(curve);
        if (curve == STEPS)
        {
            Serial.print("Number of Steps: ");
            Serial.println(n_steps);
        }
        Serial.println("----------------------------");
    }


    SetPointMode::SetPointMode()
        : setpoint(NULL_SETPOINT), value(0.0f)
    {
    }

    SETPOINTTYPE SetPointMode::getSetPoint()
    {
        return setpoint;
    }

    void SetPointMode::setSetPoint(SETPOINTTYPE setpoint)
    {
        this->setpoint = setpoint;
    }

    void SetPointMode::setSetPoint(const String& setpointStr)
    {
        this->setpoint = stringToSetPoint(setpointStr);
    }

    float SetPointMode::getValue()
    {
        return value;
    }

    void SetPointMode::setValue(float value)
    {
        this->value = value;
    }

    void SetPointMode::getParams()
    {
        Serial.println("----------------------------");
        Serial.println("Set Point Mode Parameters: ");
        Serial.print("Set Point: ");
        Serial.println(setpoint);
        Serial.print("Value: ");
        Serial.println(value);
        Serial.println("----------------------------");
    }





    SingleSpeedMode::SingleSpeedMode()
        : duty_cycle(0.0f)
    {
    }

    float SingleSpeedMode::getDutyCycle()
    {
        return duty_cycle;
    }

    void SingleSpeedMode::setTargetSpeed(long target_speed)
    {
        this->target_speed = target_speed;
    }

    long SingleSpeedMode::getTargetSpeed()
    {
        return target_speed;
    }


    void SingleSpeedMode::setDutyCycle(float duty_cycle)
    {

        if (duty_cycle >= 0 && duty_cycle <= 100)
        {
            setTargetSpeed(map(duty_cycle, 0, 100, MIN_SPEED, MAX_SPEED));
            this->duty_cycle = duty_cycle;
        }
        else
        {
            Serial.println("Invalid input! Enter a value between 0 and 100");
            this->duty_cycle = 0;
        }
    }

    void SingleSpeedMode::getParams()
    {
        Serial.println("----------------------------");
        Serial.println("Single Speed Mode Parameters: ");
        Serial.print("Duty Cycle: ");
        Serial.println(duty_cycle);
        Serial.println("----------------------------");
    }


    MODE currentMode = NULL_MODE;
    SweepMode globalSweepMode;
    SetPointMode globalSetPointMode;
    SingleSpeedMode globalSingleSpeedMode;

}

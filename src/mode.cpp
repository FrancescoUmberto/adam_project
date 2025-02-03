#include <Arduino.h>
#include "header/mode.h"

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

    int SweepMode::getInitialDC()
    {
        return initial_dc;
    }

    void SweepMode::setInitialDC(int initial_dc)
    {
        this->initial_dc = initial_dc;
    }

    int SweepMode::getFinalDC()
    {
        return final_dc;
    }

    void SweepMode::setFinalDC(int final_dc)
    {
        this->final_dc = final_dc;
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

    void SingleSpeedMode::setDutyCycle(float duty_cycle)
    {
        this->duty_cycle = duty_cycle;
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

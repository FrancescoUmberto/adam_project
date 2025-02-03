#include <Arduino.h>
#include "header/mode.h"

namespace mode {

    MODE stringToMode(const char *str)
    {
        if (strcmp(str, "SINGLE") == 0)
            return SINGLE;
        if (strcmp(str, "SWEEP") == 0)
            return SWEEP;
        if (strcmp(str, "SETPOINT") == 0)
            return SETPOINT;
        return NULL_MODE;
    }

    SETPOINTTYPE stringToSetPoint(const char *str)
    {
        if (strcmp(str, "DUTY") == 0)
            return DUTY;
        if (strcmp(str, "RPM") == 0)
            return RPM;
        if (strcmp(str, "POWER") == 0)
            return POWER;
        if (strcmp(str, "THRUST") == 0)
            return THRUST;
        return NULL_SETPOINT;
    }

    CURVE stringToCurve(const char *str)
    {
        if (strcmp(str, "RAMP") == 0)
            return RAMP;
        if (strcmp(str, "STEPS") == 0)
            return STEPS;
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

    CURVE SweepMode::getCurve()
    {
        return curve;
    }

    void SweepMode::setCurve(CURVE curve)
    {
        this->curve = curve;
    }

    void SweepMode::setCurve(const char *curveStr)
    {
        this->curve = stringToCurve(curveStr);
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

    void SetPointMode::setSetPoint(const char *setpointStr)
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


    MODE currentMode = NULL_MODE;
    SweepMode globalSweepMode;
    SetPointMode globalSetPointMode;
    SingleSpeedMode globalSingleSpeedMode;

}

#include <Arduino.h>
#include <header/mode.h>

using namespace mode;

mode::MODE stringToMode(const char *str)
{
    if (strcmp(str, "SINGLE") == 0)
        return SINGLE;
    if (strcmp(str, "SWEEP") == 0)
        return SWEEP;
    if (strcmp(str, "SETPOINT") == 0)
        return SETPOINT;
    return NULL_MODE;
}

mode::SETPOINTTYPE stringToSetPoint(const char *str)
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

MODE currentMode = MODE::NULL_MODE;

mode::CURVE stringToCurve(const char *str)
{
    if (strcmp(str, "RAMP") == 0)
        return RAMP;
    if (strcmp(str, "STEPS") == 0)
        return STEPS;
    if (strcmp(str, "GENERIC") == 0)
        return GENERIC;
    return NULL_CURVE;
}

class mode::SweepMode
{
public:
    SweepMode()
    {
        initial_dc = 0;
        final_dc = 0;
        curve = CURVE::NULL_CURVE;
    }

    int getInitialDC()
    {
        return initial_dc;
    }

    void setInitialDC(int initial_dc)
    {
        this->initial_dc = initial_dc;
    }

    int getFinalDC()
    {
        return final_dc;
    }

    void setFinalDC(int final_dc)
    {
        this->final_dc = final_dc;
    }

    CURVE getCurve()
    {
        return curve;
    }

    void setCurve(CURVE curve)
    {
        this->curve = curve;
    }

    void setCurve(const char *curve)
    {
        this->curve = mode::stringToCurve(curve);
    }

private:
    int initial_dc;
    int final_dc;
    enum CURVE curve;
};

class mode::SetPointMode
{
public:
    SetPointMode()
    {
        setpoint = SETPOINTTYPE::NULL_SETPOINT;
        value = 0;
    }

    SETPOINTTYPE getSetPoint()
    {
        return setpoint;
    }

    void setSetPoint(SETPOINTTYPE setpoint)
    {
        this->setpoint = setpoint;
    }

    void setSetPoint(const char *setpoint)
    {
        this->setpoint = mode::stringToSetPoint(setpoint);
    }

    float getValue()
    {
        return value;
    }

    void setValue(float value)
    {
        this->value = value;
    }

private:
    enum SETPOINTTYPE setpoint;
    float value;
};

class mode::SingleSpeedMode
{
public:
    SingleSpeedMode()
    {
        duty_cycle = 0;
    }

    float getDutyCycle()
    {
        return duty_cycle;
    }

    void setDutyCycle(float duty_cycle)
    {
        this->duty_cycle = duty_cycle;
    }

private:
    float duty_cycle;
};

SweepMode globalSweepMode = SweepMode();
SetPointMode globalSetPointMode = SetPointMode();
SingleSpeedMode globalSingleSpeedMode = SingleSpeedMode();
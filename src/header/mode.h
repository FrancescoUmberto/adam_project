#ifndef MODE_H
#define MODE_H

#include <Arduino.h>

namespace mode
{
    // Enumerazioni
    enum MODE
    {
        SINGLE,
        SWEEP,
        SETPOINT,
        NULL_MODE
    };
    
    enum CURVE
    {
        RAMP,
        STEPS,
        NULL_CURVE
    };

    enum SETPOINTTYPE
    {
        DUTY,
        RPM,
        POWER,
        THRUST,
        NULL_SETPOINT
    };

    MODE stringToMode(const char *str);
    SETPOINTTYPE stringToSetPoint(const char *str);
    CURVE stringToCurve(const char *str);

    class SweepMode{
    public:
        SweepMode();

        int getInitialDC();
        void setInitialDC(int initial_dc);

        int getFinalDC();
        void setFinalDC(int final_dc);

        CURVE getCurve();
        void setCurve(CURVE curve);
        void setCurve(const char *curve);

    private:
        int initial_dc;
        int final_dc;
        CURVE curve;
    };

    class SetPointMode{
    public:
        SetPointMode();

        float getValue();
        void setValue(float value);

        SETPOINTTYPE getSetPoint();
        void setSetPoint(SETPOINTTYPE setpoint);
        void setSetPoint(const char *setpoint);

    private:
        SETPOINTTYPE setpoint;
        float value;
    };

    class SingleSpeedMode{
    public:
        SingleSpeedMode();

        float getDutyCycle();
        void setDutyCycle(float duty_cycle);
    private:
        float duty_cycle;
    };

    extern MODE currentMode;
    extern SweepMode globalSweepMode;
    extern SetPointMode globalSetPointMode;
    extern SingleSpeedMode globalSingleSpeedMode;
}

#endif // MODE_H

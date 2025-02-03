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

    MODE stringToMode(const String& str);
    SETPOINTTYPE stringToSetPoint(const String& str);
    CURVE stringToCurve(const String& str);

    class SweepMode{
    public:
        SweepMode();

        int getInitialDC();
        void setInitialDC(int initial_dc);

        int getFinalDC();
        void setFinalDC(int final_dc);

        int getNSteps();
        void setNSteps(int n_steps);

        CURVE getCurve();
        void setCurve(CURVE curve);
        void setCurve(const String& curve);

        void getParams();

    private:
        int initial_dc;
        int final_dc;
        int n_steps;
        CURVE curve;
    };

    class SetPointMode{
    public:
        SetPointMode();

        float getValue();
        void setValue(float value);

        SETPOINTTYPE getSetPoint();
        void setSetPoint(SETPOINTTYPE setpoint);
        void setSetPoint(const String& setpoint);

        void getParams();

    private:
        SETPOINTTYPE setpoint;
        float value;
    };

    class SingleSpeedMode{
    public:
        SingleSpeedMode();

        float getDutyCycle();
        void setDutyCycle(float duty_cycle);

        void getParams();

    private:
        float duty_cycle;
    };

    extern MODE currentMode;
    extern SweepMode globalSweepMode;
    extern SetPointMode globalSetPointMode;
    extern SingleSpeedMode globalSingleSpeedMode;
}

#endif // MODE_H

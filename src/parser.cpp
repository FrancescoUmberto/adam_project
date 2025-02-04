#include "header/parser.h"
#include <header/mode.h>
#include "header/utils.h"

using namespace mode;
using namespace utils;

namespace parser
{

String params;
String mode_in;
float duration;
String temp_string;
float sampling_period;



    void parse_command(String &optional)
    {
        cli();
        if (substring(optional, mode_in, params))
        {
            // define MODE
            mode::currentMode = stringToMode(mode_in);
            Serial.print("Mode: ");
            Serial.println(mode_in);

            // define DURATION
            substring(params, temp_string, params);
            duration = temp_string.toFloat();
            Serial.print("Duration: ");
            Serial.println(duration);

            // define SAMPLING_PERIOD
            substring(params, temp_string, params);
            sampling_period = temp_string.toFloat();
            Serial.print("Sampling Period: ");
            Serial.println(sampling_period);

            if (mode::currentMode == MODE::SINGLE)
            {
                globalSingleSpeedMode.setDutyCycle(params.toFloat());
                Serial.print("Duty Cycle: ");
                Serial.println(globalSingleSpeedMode.getDutyCycle());

                globalSingleSpeedMode.getParams();
            }
            else if (mode::currentMode == MODE::SWEEP)
            {
                // define INITIAL_DC
                substring(params, temp_string, params);
                globalSweepMode.setInitialDC(temp_string.toInt());
                Serial.print("Initial Duty Cycle: ");
                Serial.println(globalSweepMode.getInitialDC());

                // define FINAL_DC
                substring(params, temp_string, params);
                globalSweepMode.setFinalDC(temp_string.toInt());
                Serial.print("Final Duty Cycle: ");
                Serial.println(globalSweepMode.getFinalDC());

                // define CURVE
                substring(params, temp_string, params);
                globalSweepMode.setCurve(temp_string);
                Serial.print("Curve: ");
                Serial.println(temp_string);

                if (globalSweepMode.getCurve() == CURVE::STEPS)
                {
                    // define N_STEPS
                    globalSweepMode.setNSteps(params.toInt());
                    Serial.print("Number of Steps: ");
                    Serial.println(globalSweepMode.getNSteps());
                }

                globalSweepMode.getParams();
            }
            else if (mode::currentMode == MODE::SETPOINT)
            {
                substring(params, temp_string, params);
                globalSetPointMode.setSetPoint(temp_string);
                Serial.print("Setpoint: ");
                Serial.println(temp_string);

                globalSetPointMode.setValue(params.toFloat());
                Serial.print("Value: ");
                Serial.println(globalSetPointMode.getValue());

                globalSetPointMode.getParams();
            }

            OCR1A = (16000000UL / (1024UL * sampling_period)) - 1;

;
            sei();
        }
        else
        {
            Serial.println("Parsing Error");
        }
    }
}
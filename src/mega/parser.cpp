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
        if (substring(optional, mode_in, params))
        {
            // define MODE
            mode::currentMode = stringToMode(mode_in);

            // define DURATION
            substring(params, temp_string, params);
            duration = (temp_string.toFloat())*1000;

            // define SAMPLING_PERIOD
            substring(params, temp_string, params);
            sampling_period = temp_string.toFloat();

            if (mode::currentMode == MODE::SINGLE)
            {
                globalSingleSpeedMode.setDutyCycle(params.toFloat());
            }
            else if (mode::currentMode == MODE::SWEEP)
            {
                // define INITIAL_DC
                substring(params, temp_string, params);
                globalSweepMode.setInitialDC(temp_string.toInt());

                // define FINAL_DC
                substring(params, temp_string, params);
                globalSweepMode.setFinalDC(temp_string.toInt());


                // define CURVE
                substring(params, temp_string, params);
                globalSweepMode.setCurve(temp_string);


                if (globalSweepMode.getCurve() == CURVE::STEPS)
                {
                    // define N_STEPS
                    globalSweepMode.setNSteps(params.toInt());

                }
            }
            else if (mode::currentMode == MODE::SETPOINT)
            {
                substring(params, temp_string, params);
                globalSetPointMode.setSetPoint(temp_string);


                globalSetPointMode.setValue(params.toFloat());
            }

            OCR5A = (16000000UL / (1024UL * sampling_period)) - 1;

;
        }
        else
        {
            Serial.println("Parsing Error");
        }
    }
}
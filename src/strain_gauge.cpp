#include <HX711.h>
#include <header/pin.h>
#include <header/data.h>

using namespace pin;
using namespace send_data;

namespace strain_gauge
{
    HX711 scale;
    float f;

    void begin()
    {
        scale.begin(DATA_PIN, CLOCK_PIN);
        scale.set_scale(1035.951538);
        scale.set_offset(103738.25);
        scale.tare();
    }

    void processStrainSample()
    {
        f = scale.read();
        globalData.setThrust(f);
    }

}
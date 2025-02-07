#include <header/temperature.h>
#include <header/data.h>
#include <Adafruit_MLX90614.h>

using namespace send_data;

namespace temperature
{

    Adafruit_MLX90614 mlx = Adafruit_MLX90614();

    void processTemperatureSample()
    {
        globalData.setTemperature(mlx.readObjectTempC());
    }

    void begin()
    {
        if (!mlx.begin())
        {
            Serial.println("Error connecting to MLX sensor. Check wiring.");
            while (1);
        };
    }
}



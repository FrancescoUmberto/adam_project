#ifndef DATA_H
#define DATA_H

#include <Arduino.h>

namespace send_data{

    class Data{
    public:
        Data();
        
        float getCurrent();
        void setCurrent(float current);

        float getVoltage();
        void setVoltage(float voltage);

        float getTemperature();
        void setTemperature(float temperature);

        float getRPM();
        void setRPM(float rpm);

        float getThrust();
        void setThrust(float thrust);

        float getNoise();
        void setNoise(float noise);

        void sendData();
    private:
        float current;
        float voltage;
        float temperature;
        float rpm;
        float thrust;
        float noise;
        char buffer[128];
    };

    extern Data globalData;

}

#endif 
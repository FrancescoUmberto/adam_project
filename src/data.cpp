#include <Arduino.h>
#include "header/data.h"

namespace send_data{

    char* p;

    inline void appendFloat(float value) {
        dtostrf(value, 6, 4, p);
        p += strlen(p);
        *p++ = ',';
    }

    Data::Data()
        : current(0), voltage(0), temperature(0), rpm(0), thrust(0), noise(0)
    {
        buffer[0] = 'D';
        buffer[1] = 'A';
        buffer[2] = 'T';
        buffer[3] = 'A';
        buffer[4] = ';';
    }

    float Data::getCurrent()
    {
        return current;
    }

    void Data::setCurrent(float current)
    {
        this->current = current;
    }

    float Data::getVoltage()
    {
        return voltage;
    }

    void Data::setVoltage(float voltage)
    {
        this->voltage = voltage;
    }

    float Data::getTemperature()
    {
        return temperature;
    }

    void Data::setTemperature(float temperature)
    {
        this->temperature = temperature;
    }

    float Data::getRPM()
    {
        return rpm;
    }

    void Data::setRPM(float rpm)
    {
        this->rpm = rpm;
    }

    float Data::getThrust()
    {
        return thrust;
    }

    void Data::setThrust(float thrust)
    {
        this->thrust = thrust;
    }

    float Data::getNoise()
    {
        return noise;
    }

    void Data::setNoise(float noise)
    {
        this->noise = noise;
    }

    void Data::sendData() {
    p = buffer + 5;

    appendFloat(current);
    appendFloat(voltage);
    appendFloat(temperature);
    appendFloat(rpm);
    appendFloat(thrust);

    dtostrf(noise, 6, 4, p);
    // p += strlen(p);
    // *p = '\0';

    Serial.println(buffer);
}

    

    Data globalData = Data();

}
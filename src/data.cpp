#include <Arduino.h>
#include "header/data.h"

namespace send_data{

    Data::Data()
        : current(0), voltage(0), temperature(0), rpm(0), thrust(0), noise(0)
    {
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

    void Data::sendData()
    {
    snprintf(buffer, sizeof(buffer), "DATA,%1f,%1f,%1f,%f,%.f,%f", 
            current, voltage, temperature, rpm, thrust, noise);
    
    Serial.println(buffer); // Invia l'intera stringa in una sola operazione
    }

    Data globalData = Data();

}
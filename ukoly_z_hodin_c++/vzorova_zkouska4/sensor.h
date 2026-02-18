#ifndef SENSOR_H
#define SENSOR_H
#include <string>
#include <iostream>
#include <vector>

class Sensor
{
private:
    std::string nazevSouboru;
    long aktualniCas;

public:
    Sensor(std::string soubor);
    void sens(std::string mistnost,std::string typ,std::string hodnota);
};

#endif
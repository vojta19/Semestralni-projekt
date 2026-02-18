#ifndef SMARTHOME_H
#define SMARTHOME_H
#include <string>
#include <iostream>
#include <vector>
#include "sensor.h"

class Smarthome
{
private:
    Sensor * sensor;
    std::vector<std::string> mistnost;
    std::vector<std::string> typ;
    std::vector<std::string> hodnota;
public:
    Smarthome(Sensor *s);
    void simulace(int pocetUdalosti);
};


#endif
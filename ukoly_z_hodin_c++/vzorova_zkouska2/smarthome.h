#include <iostream>
#include <string>
#include <vector>
#include "sensor.h"

class Smarthome
{
private:
    std::vector<Sensor*> senzory;
public:
    void PridatSensory(Sensor*s);
    void Simulace( int StartHodina, int PocetLogu, int krokMinuty); 
};
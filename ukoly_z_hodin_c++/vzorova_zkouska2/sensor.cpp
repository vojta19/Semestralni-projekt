#include <iostream>
#include <string>
#include <cstdlib>
#include "sensor.h"

Sensor::Sensor(std::string m,std::string t,double startHodnota)
{
    mistnost=m;
    typ=t;
    hodnota=startHodnota;
}

void Sensor::zmerit()
{
    double zmena = 0.0;
    if(typ=="HUMI")
    {
        zmena = (rand() % 5-2);
        if(hodnota<0) hodnota=0;
        if(hodnota>100) hodnota=100;
    }
    else if (typ=="TEMP")
    {
        zmena = (rand() % 10-5) /10.0;
    }
    else if (typ=="CO2")
    {
        zmena = (rand() % 31-10);
    }
    hodnota+=zmena;
}

std::string Sensor::getMistnost() {return mistnost;}
std::string Sensor::getTyp() {return typ;}
double Sensor::getHodnota() {return hodnota;}

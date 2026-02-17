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
    double zmena = (rand() % 10-5) /10.0;
    hodnota += zmena;

    if(typ=="HUMI")
    {
        if(hodnota<0) hodnota=0;
        if(hodnota>100) hodnota=100;
    }
}

std::string Sensor::getMistnost() {return mistnost;}
std::string Sensor::getTyp() {return typ;}
double Sensor::getHodnota() {return hodnota;}

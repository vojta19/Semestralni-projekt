#ifndef SMARTHOME_H
#define SMARTHOME_H
#include <string>
#include <iostream>
#include <vector>
#include "sensor.h"

class Smarthome                 //třída pro simulaci chodu chytré domácnosti
{
private:
    Sensor * sensor;                    //ukazatel na objekt třídy Sensor, který se používá pro načítání dat ze senzorů
    std::vector<std::string> mistnost;  //vektor pro uložení názvů místností, ve kterých jsou umístěny senzory
    std::vector<std::string> typ;       //vektor pro uložení typů senzorů (teplota a vlhkost)
    std::vector<std::string> hodnota;   //vektor pro uložení hodnot senzorů, které jsou generovány během simulace
public:
    Smarthome(Sensor *s);               //konstruktor třídy Smarthome, který přijímá ukazatel na objekt Sensor a inicializuje vektory pro místnosti a typy senzorů
    void simulace(int pocetUdalosti);   //metoda simulace, která generuje náhodné události pro zadaný počet událostí, včetně výběru náhodné místnosti, typu senzoru a generování náhodné hodnoty senzoru, a volá metodu sens objektu Sensor pro záznam události do souboru
};


#endif
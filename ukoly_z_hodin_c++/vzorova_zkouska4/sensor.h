#ifndef SENSOR_H
#define SENSOR_H
#include <string>
#include <iostream>
#include <vector>

class Sensor            //třída pro zpracování dat ze senzorů
{
private:
    std::string nazevSouboru;   //název souboru, ze kterého se načítají data
    long aktualniCas;           //aktuální čas, který se aktualizuje při každém načtení dat ze souboru

public:
    Sensor(std::string soubor); //konstruktor, který načítá data ze souboru a aktualizuje aktuální čas
    void sens(std::string mistnost,std::string typ,std::string hodnota); 
    //metoda pro zpracování dat ze senzorů, která přijímá název místnosti, typ senzoru a hodnotu senzoru
};

#endif
#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include "sensor.h"

Sensor::Sensor(std::string soubor)
{
    nazevSouboru = soubor;
    aktualniCas = 480;
}

void Sensor::sens(std::string mistnost, std::string typ, std::string hodnota)
{
    std::ofstream soubor;
    soubor.open(nazevSouboru,std::ios::app);
    if(soubor.is_open())
    {
        int hodiny=(aktualniCas/60)%24;
        int minuty= aktualniCas%60;
        soubor << "[" <<hodiny<< ":" <<std::setw(2)<<std::setfill('0')<<minuty<< "] " << mistnost << ": " << typ << " - " <<hodnota << std::endl;
        aktualniCas+=15;
        soubor.close();
    }
    else
    {
        std::cout << "chyba, soubor nelze otevrit" << std::endl;
    }
}
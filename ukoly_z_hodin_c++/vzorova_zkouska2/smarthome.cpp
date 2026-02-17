#include <iostream>
#include <iomanip>
#include <string>
#include "smarthome.h"
#include <fstream>

void Smarthome::PridatSensory(Sensor*s)
{
    senzory.push_back(s); //přidání senzoru do vektoru a na konec
}

void Smarthome::Simulace(int startHodina, int PocetLogu, int krokMinuty,std::string nazevSouboru)
{
    std::ofstream soubor;
    soubor.open(nazevSouboru,std::ios::app);

    if(!soubor.is_open())
    {
        std::cout<<"Chyba soubor nelze otevrit"<<std::endl;
        return;
    }
    
    //převod hodin na minuty
    int celkoveMinuty=startHodina*60;

    for(int i=0;i<PocetLogu;i++)
    {
        int h= (celkoveMinuty/60)%24;      //zisk hodin
        int m= (celkoveMinuty%60);         //zisk minut

        //projdeme všechny sensory
        for(Sensor*s:senzory)
        {
            s->zmerit(); //aktualizace hodnoty

            soubor  <<h<<":"
                    <<std::setw(2)<<std::setfill('0')<<m<<" "       //setw(2) a setfill(0) kvůli formátu 8:05
                    <<s->getMistnost()<<" "
                    <<s->getTyp()<<" "
                    <<std::fixed<<std::setprecision(1)<<s->getHodnota()     //std::fixed a std::setprecision kvůli desetinnému formátu, např. 22.4
                    <<std::endl;
            std::cout << "Zapsano do logu: " << h << ":" << std::setw(2) << std::setfill('0') << m << "..." << std::endl;
        }
        celkoveMinuty+=krokMinuty;
    }

    soubor.close();
}
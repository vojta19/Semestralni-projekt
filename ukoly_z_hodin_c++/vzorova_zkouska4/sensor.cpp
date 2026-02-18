#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include "sensor.h"

Sensor::Sensor(std::string soubor) //konstruktor
{
    nazevSouboru = soubor;        //ulozeni nazvu souboru do atributu
    aktualniCas = 480;              //nastaveni aktualniho casu na 8:00 (480 minut od pulnoci)
}

void Sensor::sens(std::string mistnost, std::string typ, std::string hodnota)   //metoda pro zapis dat do souboru
{
    std::ofstream soubor;                       //vytvoreni objektu pro zapis do souboru
    soubor.open(nazevSouboru,std::ios::app);    //otevreni souboru pro zapis (append mode)
    if(soubor.is_open())                        //kontrola, zda se soubor otevrel uspesne
    {
        int hodiny=(aktualniCas/60)%24;         //vypocet hodin z aktualniho casu
        int minuty= aktualniCas%60;             //vypocet minut z aktualniho casu
        soubor << "[" <<hodiny<< ":" <<std::setw(2)<<std::setfill('0')<<minuty<< "] " << mistnost << ": " << typ << " - " <<hodnota << std::endl;
        //zapsani dat do souboru ve formatu [hh:mm] mistnost: typ - hodnota
        aktualniCas+=15;    //posun casu o 15 minut pro dalsi zaznam
        soubor.close();     //uzavreni souboru po zapsani dat
    }
    else
    {
        std::cout << "chyba, soubor nelze otevrit" << std::endl;    //vypis chybove hlasky, pokud se soubor nepodarilo otevrit
    }
}
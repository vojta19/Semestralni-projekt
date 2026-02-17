#include "logger.h"
#include <fstream>
#include <iostream>

Logger::Logger(std::string soubor) 
{
    nazevSouboru = soubor;  //název souboru do atributu třídy
    aktualniCas = 1000;     //počáteční simulovaný čas
}

void Logger::log(std::string uroven, std::string objekt, std::string zprava) 
{
    std::ofstream soubor;                           //vytvoříme proud pro zápis do souboru
    soubor.open(nazevSouboru, std::ios::app);       //otevřeme soubor a důležitá funkce append, která zapíše na konec soubor a nepřepíše stávající obsah

    if (soubor.is_open()) //kontrola otevření
    {
        //soubor ve formátu [CAS] LEVEL: Objekt - Zpráva
        soubor << "[" << aktualniCas << "] " 
               << uroven << ": " 
               << objekt << " - " 
               << zprava << std::endl;
        
        aktualniCas++; //zvýšíme počítadlo, aby další log měl jiné číslo
        
        soubor.close(); //zavřeme soubor
    } 
    else 
    {
        std::cerr << "Chyba: Nelze otevřít soubor pro zápis!" << std::endl; //když se neotevře, vypíšeme chybovou hlášku
    }
}
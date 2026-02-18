#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "logger2.h"

Logger2::Logger2(std::string soubor)
{
    nazevSouboru = soubor;
    aktualniCas=17000;
}

void Logger2::log(std::string uroven,std::string objekt,std::string zprava)
{
    std::ofstream soubor(nazevSouboru,std::ios::app);
    if(soubor.is_open())
    {
        soubor << "[" << aktualniCas << "] " << uroven << ": "<< objekt << " - " << zprava << std::endl;
        
        aktualniCas++;
        soubor.close();
    }
    else
    {
        std::cout << "Nelze otevrit soubor pro zapis." <<std::endl;
    }
}

Server::Server(Logger2*l)
{
    log=l;
}

void Server::run(int pocetUdalosti)
{
    std::cout << "Server bezi a generuje" << pocetUdalosti << "udalosti..." << std::endl;

    std::vector<std::string> urovne = {"INFO","WARNING","ERROR"};
    std::vector<std::string> objekty = {"Databaze","Server","System"};
    std::vector<std::string> zpravy = {"Start procesu","Uzivatel prihlasen","Zaloha dokoncena","Vysoka odezva","Nizka pamet","Neznamy pozadavek","Spojeni selhalo",
                            "Segfault","Disk plny"};

    for(int i=0;i<pocetUdalosti;i++)
    {
        int indexUroven = rand() % urovne.size();
        int indexObjekt = rand() % objekty.size();

        int zacatekZpravy = indexUroven * 3;
        int posun = rand() % 3;
        int indexZprava = zacatekZpravy + posun;

        log->log(urovne[indexUroven],objekty[indexObjekt],zpravy[indexZprava]);
    }

    std::cout << "Generovani dokonceno." << std::endl;
}
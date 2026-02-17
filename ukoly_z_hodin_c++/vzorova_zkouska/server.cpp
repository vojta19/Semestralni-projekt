#include <iostream>
#include <cstdlib>  //kvůli rand()
#include "server.h"

Server::Server(Logger *l)
{
    logger=l;               //uložíme si odkaz na logger, který přišel z mainu

    //naplníme vektory daty
    urovne = {"INFO","WARNING","ERROR"};
    objekty = {"Databaze","Server","System"};

    //seznam zpráv, důležité pořadí (0-2 info, 3-5 urovne, 6-8 error)
    zpravy = {"Start procesu","Uzivatel prihlasen","Zaloha dokoncena","Vysoka odezva","Nizka pamet","Neznamy pozadavek","Spojeni selhalo","Segfault","Disk plny"};
}

void Server::run(int pocetUdalosti)
{
    std::cout << "Server bezi a generuje " << pocetUdalosti << " logu..." << std::endl; //hláška do konzole

        for (int i = 0; i < pocetUdalosti; i++) //cyklus
        {
            int indexUrovne = rand() % urovne.size(); //náhodně vybereme index úrovně (0,1,2)
            int indexObjektu = rand() % objekty.size(); //náhodně vybereme index objektu (0,1,2)

            int zacatekSekce = indexUrovne * 3;         //pokud je úroveň 0 začátek je 0, pokud 1 začátek je 3, pokud 2 začátek je 6
            int posun = rand() % 3;                    //k začátku přičteme náhodně jednu z hodnot 0 až 2, abychom vybrali jednu ze zpráv
            int indexZpravy = zacatekSekce + posun;
            
            //zavoláme metodu log na objektu logger, použijeme šipku, protože logger je pointer
            logger->log(urovne[indexUrovne], objekty[indexObjektu], zpravy[indexZpravy]);
        }

        std::cout << "Generovani dokonceno." << std::endl;
}
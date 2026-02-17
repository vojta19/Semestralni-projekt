#ifndef SERVER_H
#define SERVER_H

#include "logger.h" //server musí znát logger
#include <vector>
#include <string>

class Server
{
private:
    Logger*logger;                         //ukazatel na objekt Loggeru, pracujeme se stávajícím, ne novým
    std::vector<std::string> urovne;       //seznamy textů, ze kterých budeme vybírat
    std::vector<std::string> objekty;
    std::vector<std::string> zpravy;

public:
    Server(Logger *l);              //konstruktor přijímá ukazatel na Logger
    void run(int pocetUdalosti);    //metoda spuštění generovaní logů
};

#endif

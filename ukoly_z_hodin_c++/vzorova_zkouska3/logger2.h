#ifndef LOGGER2_H
#define LOGGER2_H
#include <string>
#include <iostream>
#include <vector>

class Logger2
{
private:
    std::string nazevSouboru;
    long aktualniCas;
public:
    Logger2(std::string soubor);
    void log(std::string uroven,std::string objekt,std::string zprava);
};

class Server
{
private:
    Logger2*log;
public:
    Server(Logger2*l);
    void run(int pocetUdalosti);
};

#endif
#ifndef LOGGER_H  //ochrana proti vícenásobnému otevření souboru
#define LOGGER_H  // pokud je už LOGGER_H definován, zbytek se přeskočí

#include <string> //textové řetězce
class Logger //definice třídy Logger
{
private:
    std::string nazevSouboru;  //sem si uložíme jméno souboru např. server_log
    long aktualniCas;          //simulovaný čas, aby logy nebyly stejné

public:
    Logger(std::string soubor); //konstruktor, volá se při vytvoření objektu, nastaví jméno souboru
    void log(std::string uroven, std::string objekt, std::string zprava);  //hlavní metoda pro zprávu, přijme úroveň, objekt a zprávu
};

#endif //konec ochrany
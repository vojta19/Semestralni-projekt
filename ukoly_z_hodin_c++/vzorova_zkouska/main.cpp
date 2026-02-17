#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include "logger.h"
#include "server.h"

int main()
{
    srand(time(0)); //inicializace generátoru náhodných čísel podle aktuálního času
    Logger mujLogger("server.log"); //vytvoření instance Loggeru, otevře nebo vytvoří soubor
    Server mujServer(&mujLogger); //vytvoření insance Serveru, přiřadíme mu adresu našeho loggeru aby ho mohl používat
    mujServer.run(50);            //generování 50 řádků

    return 0;
}
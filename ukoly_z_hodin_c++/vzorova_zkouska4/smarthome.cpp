#include <iostream>
#include <string>
#include <iomanip>
#include "smarthome.h"
#include <cstdlib>

Smarthome::Smarthome(Sensor*s)
{
    sensor = s;
    mistnost = {"BEDROOM", "LIVING ROOM", "KITCHEN"};
    typ = {"TEMP","HUMI"};
    
}

void Smarthome::simulace(int pocetUdalosti)
{
    std::cout<<"Server bezi a provadi " << pocetUdalosti << " udalosti." << std::endl;
    for(int i=0;i<pocetUdalosti;i++)
    {
        int indexMistnosti = rand() % mistnost.size();
        int indexTypu = rand() % typ.size();
        double vypocitanaHodnota = 0.0;


        if(typ[indexTypu] == "TEMP")
        {
            vypocitanaHodnota = (220+(rand() % 71)) / 10.0;
        }
        else if (typ[indexTypu] == "HUMI")
        {
            vypocitanaHodnota = (500+(rand() % 201)) /10.0;
        }

        std::string hodnota = std::to_string(vypocitanaHodnota);

        sensor->sens(mistnost[indexMistnosti], typ[indexTypu],hodnota);
    }

    std::cout << "Simulace ukoncena." << std::endl;
}

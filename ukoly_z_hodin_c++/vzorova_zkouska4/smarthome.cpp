#include <iostream>
#include <string>
#include <iomanip>
#include "smarthome.h"
#include <cstdlib>
#include <sstream>

Smarthome::Smarthome(Sensor*s)  // Konstruktor třídy Smarthome, který přijímá ukazatel na objekt Sensor a inicializuje vektory pro místnosti a typy senzorů
{
    sensor = s;
    mistnost = {"BEDROOM", "LIVING ROOM", "KITCHEN"};   // Inicializace vektoru místností
    typ = {"TEMP","HUMI"};                              // Inicializace vektoru typů senzorů (teplota a vlhkost)
    
}

void Smarthome::simulace(int pocetUdalosti)             // Metoda simulace, která generuje náhodné události pro zadaný počet událostí
{
    std::cout<<"Server bezi a provadi " << pocetUdalosti << " udalosti." << std::endl;  // Výpis informace o spuštění simulace a počtu událostí, které budou generovány
    for(int i=0;i<pocetUdalosti;i++)    // Smyčka pro generování náhodných událostí, která probíhá po zadaný počet událostí
    {
        int indexMistnosti = rand() % mistnost.size();  // Generování náhodného indexu pro výběr místnosti z vektoru místností
        int indexTypu = rand() % typ.size();            // Generování náhodného indexu pro výběr typu senzoru z vektoru typů senzorů
        double vypocitanaHodnota = 0.0;             // Proměnná pro uložení vypočítané hodnoty senzoru, která bude generována na základě typu senzoru


        if(typ[indexTypu] == "TEMP")        // Pokud je vybraný typ senzoru "TEMP", generuje se náhodná hodnota teploty v rozmezí 22.0 až 29.9 stupňů Celsia
        {
            vypocitanaHodnota = (220+(rand() % 71)) / 10.0; // Generování náhodné hodnoty teploty a její převod na desetinné číslo s jedním desetinným místem
        }
        else if (typ[indexTypu] == "HUMI")                  // Pokud je vybraný typ senzoru "HUMI", generuje se náhodná hodnota vlhkosti v rozmezí 50.0 až 70.0 procent
        {
            vypocitanaHodnota = (500+(rand() % 201)) /10.0; // Generování náhodné hodnoty vlhkosti a její převod na desetinné číslo s jedním desetinným místem
        }

        std::stringstream stream;                           // Vytvoření objektu stringstream pro formátování vypočítané hodnoty senzoru
        stream << std::fixed << std::setprecision(1) << vypocitanaHodnota;  // Nastavení formátu pro výstup hodnoty senzoru s jedním desetinným místem
        std::string hodnota = stream.str();                                 // Převod formátované hodnoty senzoru na řetězec
        sensor->sens(mistnost[indexMistnosti], typ[indexTypu],hodnota);     // Volání metody sens objektu Sensor pro záznam události do souboru, předání místnosti, typu senzoru a vypočítané hodnoty jako argumentů
        std::cout << "Udalost " << i+1 << ": " << mistnost[indexMistnosti] << " - " << typ[indexTypu] << " = " << hodnota << std::endl;
        // Výpis informace o generované události, včetně čísla události, místnosti, typu senzoru a vypočítané hodnoty senzoru
    }

    std::cout << "Simulace ukoncena." << std::endl;
}

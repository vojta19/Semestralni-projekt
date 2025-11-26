#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

using namespace std;                        //  pro zkrácení zápisu map, set, string, ...
using InnerMap = map<string, int>;          //  vnitřní mapa - klíč: barva, hodnota: počet
using Rules = map<string, InnerMap>;        //  pravidla - klíč: vnější barva, hodnota: vnitřní mapa

Rules pravidla;
std::set<std::string>gold;

std::string trim(const std::string&str)     // funkce pro ořezání mezer z obou stran řetězce
{
    size_t first = str.find_first_not_of(' ');  // najdi první znak, který není mezera
    if (std::string::npos == first) return str; // pokud jsou všechny znaky mezery, vrať původní řetězec
    size_t last = str.find_last_not_of(' ');      // najdi poslední znak, který není mezera
    return str.substr(first, (last - first + 1));   // vrať ořezaný řetězec
}


void printRules()
{
    for (const auto& pair : pravidla)       // iteruj přes všechna pravidla
    {
        const std::string& outer = pair.first;  // vnější barva
        const InnerMap& contents = pair.second; // vnitřní mapa
        std::cout << outer << " bags contain: ";    // vypiš vnější barvu
    for (const auto& contentPair : contents)        // iteruj přes vnitřní mapu
    {      
        const std::string& inner = contentPair.first;   // vnitřní barva
        const int count = contentPair.second;           // počet
        std::cout << count << " " << inner << ", ";     // vypiš počet a vnitřní barvu
    }
    std::cout << std::endl;
    }
}

void parseRules(const std::string&filename)
{
        std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Chyba: Soubor nelze otevřít: " + filename);
    }
    std::string line;
    while (std::getline(file, line)) 
    {
        size_t containPos = line.find(" bags contain ");        // najdi pozici " bags contain "
        if (containPos == std::string::npos) continue;          // pokud není nalezeno, pokračuj na další řádek
        std::string outerKey = line.substr(0, containPos);      // vnější barva
        std::string contentsPart = line.substr(containPos + 14);    // část s obsahem
        contentsPart.pop_back();                        // odstraň tečku na konci   
        std::stringstream ssContents(contentsPart);     // vytvoř stringstream pro obsah
        std::string segment;                        // jednotlivé segmenty obsahu
        while (std::getline(ssContents, segment, ','))  // rozděl podle čárek
        {
            segment = trim(segment);                    // ořež mezery
            std::stringstream ssSegment(segment);       // vytvoř stringstream pro segment
            std::string sCount, adj, color;                 // počet, přídavné jméno, barva
            ssSegment >> sCount >> adj >> color;        // načti počet, přídavné jméno a barvu
            if (sCount == "no") 
            {
                // žádné obaly uvnitř
            } 
            else 
            {
                std::string innerKey = adj + " " + color;   // vnitřní barva
                // ulož pravidlo do mapy
                pravidla[outerKey][innerKey] = std::stoi(sCount);   // převedení počtu na int a uložení do vnitřní mapy
            }
        }
    }
}
void find_1(std::string hledany)            // rekurzivní funkce pro hledání všech obalů, které mohou obsahovat hledaný obal
{
    for (const auto& pair : pravidla)       // iteruj přes všechna pravidla
    {
        const std::string& pravidlo = pair.first;       // vnější barva
        const InnerMap& obsah = pair.second;            // vnitřní mapa
        if (obsah.find(hledany) != obsah.end())         // pokud vnitřní mapa obsahuje hledaný obal
        {
            if (gold.find(pravidlo) == gold.end())      // pokud jsme tento obal ještě nepřidali do množiny
            {
                gold.insert(pravidlo);                  // přidej obal do množiny
                find_1(pravidlo);                       // rekurzivně hledej obaly, které mohou obsahovat tento obal
            }
        }
    }
}

long long find_2(std::string hledany)           // rekurzivní funkce pro spočítání všech obalů uvnitř hledaného obalu
{
    long long suma = 0;                         // inicializuj sumu na 0
    for(const auto& vnitrni : pravidla[hledany])        // iteruj přes všechny vnitřní obaly hledaného obalu
    {
        if(vnitrni.first=="other bags")                 // pokud je to "other bags", pokračuj dál
        {
            continue;                                   // přeskoč tento případ
        }
        suma += vnitrni.second*find_2(vnitrni.first);       // přidej počet vnitřních obalů násobený počtem obalů uvnitř nich
        suma += vnitrni.second;                              // přidej počet vnitřních obalů
    }
    return suma;
}
int main()
{
    try
    {
        parseRules("pravidla.txt");
        printRules();
        find_1("shiny gold");
        std::cout << gold.size() << std::endl;
        long long pocet = find_2("shiny gold");
        std::cout << pocet << std::endl;   
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
#include <iostream>

using namespace std;

void najdi_nejmensi_nejvetsi(int* pole, int velikost_pole, int* nejmensi, int* nejvetsi)
{
    int* stop=pole + velikost_pole; //ukazatel na konec pole
    *nejmensi = *pole; //inicializace nejmensi a nejvetsi hodnoty prvnim prvkem pole
    *nejvetsi = *pole;  //inicializace nejmensi a nejvetsi hodnoty prvnim prvkem pole
    while(pole != stop) //prochazi cele pole, dokud nenarazi na nejmensi a nejvetsi cislo
    {
        std::cout << "adresa pole: " << pole << std::endl;
        std::cout << "hodnota na adrese je: " << *pole << std::endl;
        if(*nejmensi>*pole) //pokud je aktualni hodnota mensi nez nejmensi nalezena hodnota
        {
            *nejmensi = *pole; //nastaveni nove nejmensi hodnoty
        }
        if(*nejvetsi<*pole) //pokud je aktualni hodnota vetsi nez nejvetsi nalezena hodnota
        {
            *nejvetsi = *pole; //nastaveni nove nejvetsi hodnoty
        }
        pole++; //posun na dalsi prvek pole
    }
}

int main()
{
    int pole[10] = {9,4,6,27,3,15,8,12,2,20};
    int nejmensi;
    int nejvetsi;
    najdi_nejmensi_nejvetsi(pole, 10, &nejmensi, &nejvetsi);
    cout << "Nejmensi cislo je: " << nejmensi <<endl;
    cout << "Nejvetsi cislo je: " << nejvetsi <<endl;
    return 0;
}
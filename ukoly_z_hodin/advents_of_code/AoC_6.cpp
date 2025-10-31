#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

//Advents of Code, rok 2021, den 6, vektory
//vstupní hodnoty: 3,4,3,1,2  -> úkol je, kolik jich bude po 80 dnech
//každý den se sníží o 1, pokud je 0, tak se nastaví na 6 a přidá se nový s hodnotou 8

int main()
{
    ifstream soubor("ryby.txt");
    if(!soubor.is_open())
    {
        cout<<"Soubor se nepodarilo otevrit!"<<endl;
        return 1;
    }
    string radek;
    vector<int> ryby;
    while(getline(soubor,radek))
    {
        size_t pos=0;                               //pozice čárky
        string token;                               //jednotlivé číslo jako string
        while((pos=radek.find(','))!=string::npos)  //dokud najde čárku
        {
            token=radek.substr(0,pos);              //uloží číslo do tokenu
            ryby.push_back(stoi(token));            //převede na int a uloží do vektoru
            radek.erase(0,pos+1);                   //odstraní z řetězce zpracované číslo a čárku
        }
        ryby.push_back(stoi(radek));                //uloží poslední číslo (za poslední čárkou)
    }
    soubor.close();

    while(true)
    {
        int pocetNovych=0;                      //počítadlo nových ryb
        for(size_t i=0;i<ryby.size();i++)       //projde všechny ryby
        {
            if(ryby[i]==0)                      //pokud je na nule
            {
                ryby[i]=6;          //nastaví na 6
                pocetNovych++;      //přičte novou rybu
            }
            else                    
            {
                ryby[i]--;          //jinak sníží o 1
            }
        }
        for(int j=0;j<pocetNovych;j++)      //přidá nové ryby na konec vektoru
        {
            ryby.push_back(8);              //každá nová ryba začíná na 8
        }
        static int den=0;                   //počítadlo dnů
        den++;                              //zvýší o 1
        if(den==80)                         //po 80 dnech ukončí cyklus
        {
            break;                          //ukončí cyklus
        }
    }

    cout<<"Po 80 dnech je ryb: "<<ryby.size()<<endl;
    return 0;
}
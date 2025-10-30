#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Ponorka
{
    public:
    int horizontal=0;
    int hloubka=0;
    int aim=0;

    void ZpracujPrikaz(const string &prikaz, int vzdalenost)
    {
        if(prikaz=="forward")
        {
            if(aim!=0)
            {
                hloubka+=aim*vzdalenost;
            }
            horizontal+=vzdalenost;
        }
        if(prikaz=="down")
        {
            aim+=vzdalenost;
        }
        if(prikaz=="up")
        {
            aim-=vzdalenost;
        }

    }
};

int main()
{
    ifstream soubor("data.txt");
    if(!soubor.is_open())
    {
        cout<<"Soubor se nepodarilo otevrit!"<<endl;
        return 1;
    }
    
    Ponorka ponorka;
    string prikaz;
    int vzdalenost;
    while(soubor>>prikaz>>vzdalenost)
    {
        ponorka.ZpracujPrikaz(prikaz,vzdalenost);
    }
    soubor.close();

    cout<<"Horizontal: "<<ponorka.horizontal<<endl;
    cout<<"Hloubka: "<<ponorka.hloubka<<endl;
    cout<<"Vysledek: "<<ponorka.horizontal*ponorka.hloubka<<endl;
    return 0;

}

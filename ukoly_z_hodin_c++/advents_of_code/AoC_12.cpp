#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Lod
{
    private:
    int x=0;
    int y=0;
    int smer;
    int stupne=90;
    std::string filename;
    char stupneNaSmer()
    {
        switch(stupne)
        {
            case 0:
            return 'N';
            break;

            case 90:
            return 'E';
            break;

            case 180:
            return 'S';
            break;

            case 270:
            return 'W';
            break;

            default:
            return ' ';
            break;
        }
    }

    public:
    Lod(std::string filename)
    {
        this->filename=filename;
    }
    void Navigace()
    {
        std::string radek;
        std::ifstream soubor(filename);
        if(!soubor.is_open())
        {
            cout<<"Soubor se nepodarilo otevrit!"<<endl;
            return;
        }
        
        while (std::getline(soubor, radek))
        {
            if(!radek.empty())
            {
                char prikaz=radek[0];
                int hodnota=std::stoi(radek.substr(1));
                if(prikaz=='F')
                {
                    PosunDopredu(hodnota);
                }
                else if(prikaz=='L' || prikaz=='R')
                {
                    ZmenaSmeru(prikaz,hodnota);
                }
                else
                {
                    PosunSmerem(prikaz,hodnota);
                }
            }
        }
        soubor.close();
    }
    void ZmenaSmeru(char smer, int stupne)
    {
        switch(smer)
        {
            case 'L':
            this->stupne=(this->stupne-stupne)%360;
            break;

            case 'R':
            this->stupne=(this->stupne+stupne)%360;
            break;

            if(this->stupne<0)
            {
                this->stupne+=360;
            }

            default:
            break;
        }
    }
    void PosunDopredu(int kroky)
    {
        PosunSmerem(stupneNaSmer(),kroky);
    }
    void PosunSmerem(char smer, int kroky)
    {
        switch (smer)
        {
            case 'N':
            y+=kroky;
            break;

            case 'S':
            y-=kroky;
            break;

            case 'E':
            x+=kroky;
            break;

            case 'W':
            x-=kroky;
            break;

            default:
            break;
        }
    }
    void VypisPozice()
    {
        cout<<"Pozice x je: "<<x<<" a pozice y je: "<<y<<" a smer je: "<<stupne<<endl;
    }
};

int main()
{
    Lod lod ("test.txt");
    lod.Navigace();
    lod.VypisPozice();
    return 0;
}
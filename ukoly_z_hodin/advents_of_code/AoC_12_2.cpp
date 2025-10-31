#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

class Waypoint
{
    private:
    int x;
    int y;
    
    public:
    Waypoint(int startX, int startY) : x(startX), y(startY) {}
    void VypisPozice() const
    {
        cout<<"Waypoint pozice x: "<<x<<" y: "<<y<<endl;
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
        }
    }

    void Otocit(char smer, int stupne)
    {
       if(stupne==180)
        {
            x=-x;
            y=-y;
        }
         else if((smer=='R' && stupne==90) || (smer=='L' && stupne==270))
        {
            int temp=x;
            x=y;
            y=-temp;
        }
         else if((smer=='L' && stupne==90) || (smer=='R' && stupne==270))
        {
            int temp=x;
            x=-y;
            y=temp;
        }
    }

    int getX() const { return x; }
    int getY() const { return y; }
};

class Lod
{
    private:
    int x;
    int y;

    Waypoint waypoint;
    
    public:
    Lod(): x(0),y(0), waypoint(10,1) {}

    void ZpracujPrikaz(char prikaz, int hodnota)
    {
        switch(prikaz)
        {
            case 'F':
                x += waypoint.getX() * hodnota;
                y += waypoint.getY() * hodnota;
                break;
            case 'N':
                waypoint.PosunSmerem(prikaz, hodnota);
                break;
            case 'S':
                waypoint.PosunSmerem(prikaz, hodnota);
                break;
            case 'E':
                waypoint.PosunSmerem(prikaz, hodnota);
                break;
            case 'W':
                waypoint.PosunSmerem(prikaz, hodnota);
                break;
            case 'L':
                waypoint.Otocit(prikaz, hodnota);
                break;
            case 'R':
                waypoint.Otocit(prikaz, hodnota);
                break;
            default:
                break;
        }
    }
    void Navigace()
    {
        ifstream soubor("test.txt");
        if (!soubor.is_open())
        {
        cout << "Soubor se nepodarilo otevrit!" << endl;
        return;
        }
        string radek;
        while (getline(soubor, radek))
        {
            char prikaz = radek[0];
            int hodnota = stoi(radek.substr(1));
            ZpracujPrikaz(prikaz, hodnota);
        }

        soubor.close();
    }

    int ManhattanskaVzdalenost() const
    {
        return abs(x) + abs(y);
    }

};

int main()
{
    Lod lod;
    lod.Navigace();
    cout<<"Manhattanska vzdalenost: "<<lod.ManhattanskaVzdalenost()<<endl;
    return 0;
}
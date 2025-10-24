#include <iostream>
#include <string>

using namespace std;

void VypisStromecek(int vyska)
{
    for(int i = 1; i < vyska; i++)
    {
        int mezery=vyska - i;
        for(int j = 0; j < mezery; ++j)
        {
            cout << " ";
        }
        // Vypis hvezdicek
        for(int k = 0; k < (2 * i - 1); ++k)
        {
            cout << "*";
        }
        cout << endl;
    }
}

int main()
{
    int vyska;
    int mezery;
    cout << "Zadejte vysku stromecku: " <<flush;
    string vstup;
    getline(cin, vstup);
    vyska=stoi(vstup);
    VypisStromecek(vyska);
    return 0;
}
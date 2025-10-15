#include <iostream>
#include <cmath>

using namespace std;

void VypocetBMI() 
{
    double vaha, vyska;
    double bmi;

    cout << "Zadejte vahu (v kg): ";
    cin >> vaha;
    cout << "Zadejte vysku (v metrech): ";
    cin >> vyska;

    if (vyska <= 0) {
        cout << "Vyska musi byt vetsi nez nula." << endl;
        return;
    }

    bmi = vaha / (vyska * vyska);
    cout << "Vase BMI je: " << bmi << endl;

    if (bmi < 18.5) {
        cout << "Podvaha" << endl;
    } else if (bmi >= 18.5 && bmi < 24.9) {
        cout << "Normalni vaha" << endl;
    } else if (bmi >= 25 && bmi < 29.9) {
        cout << "Nadvaha" << endl;
    } else {
        cout << "Obezita" << endl;
    }
}

void VypocetBMR() 
{
    double hmotnost, vyska, vek, bmr;
    char pohlavi;
    cout << "Zadejte svou hmotnost v kg: ";
    cin >> hmotnost;
    cout << "Zadejte svou vysku v cm: ";
    cin >> vyska;
    cout << "Zadejte svuj vek: ";
    cin >> vek;
    cout << "Zadejte sve pohlavi (M pro muze, Z pro zeny): ";
    cin >> pohlavi;

    if (pohlavi == 'M' || pohlavi == 'm') {
        bmr = 66.47 + (13.75 * hmotnost) + (5.003 * vyska) - (6.775 * vek);
    } else if (pohlavi == 'Z' || pohlavi == 'z') {
        bmr = 655.1 + (9.563 * hmotnost) + (1.850 * vyska) - (4.676 * vek);
    } else {
        cout << "Neplatne pohlavi." << endl;
        return;
    }

    cout << "Vas bazalni metabolicky vydej (BMR) je: " << bmr << " kcal/den" << endl;
}

int main() 
{
    int volba;
    do 
    {
    cout << "Fitness Kalkulacka" << endl;
    cout << "1. Vypocet BMI" << endl;
    cout << "2. Vypocet BMR" << endl;
    cout << "3. Konec" << endl;
    cout << "Zadejte svou volbu (1-3): ";
    cin >> volba;

    switch (volba) 
    {
        case 1:
            VypocetBMI();
            break;
        case 2:
            VypocetBMR();
            break;
        case 3:
            cout << "Dekuji za pouziti a nashledanou" << endl;
            break;
        default:
            cout << "Neplatna volba, zadejte prosim cislo 1-3" << endl;
            break;
    }
    cout << endl;
    } while (volba != 3);

    return 0;
}
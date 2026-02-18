#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <random>
#include <iomanip>
#include <ctime>
#include "smarthome.h"
#include "sensor.h"

int main()
{
    srand(time(0));                     // Inicializace generátoru náhodných čísel
    Sensor mujSensor("zkouska.log");    // Vytvoření instance třídy Sensor s názvem souboru "zkouska.log"
    Smarthome mujSmarthome(&mujSensor); // Vytvoření instance třídy Smarthome a předání ukazatele na objekt Sensor
    mujSmarthome.simulace(50);          // Spuštění simulace s 50 událostmi
    return 0;
}
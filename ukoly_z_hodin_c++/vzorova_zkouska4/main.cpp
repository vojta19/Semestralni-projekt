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
    srand(time(0));
    Sensor mujSensor("zkouska.log");
    Smarthome mujSmarthome(&mujSensor);
    mujSmarthome.simulace(10);
    return 0;
}
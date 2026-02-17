#include <iostream>
#include <ctime>
#include "smarthome.h"
#include "sensor.h"

int main()
{
    srand(time(0));

    Smarthome mujDum;

    Sensor s1("BEDROOM","TEMP",22.0);
    Sensor s2("BEDROOM","HUMI",60.0);
    Sensor s3("KITCHEN","TEMP",24.5);
    Sensor s4("LIVING","TEMP",23.0);

    mujDum.PridatSensory(&s1);
    mujDum.PridatSensory(&s2);
    mujDum.PridatSensory(&s3);
    mujDum.PridatSensory(&s4);

    std::cout<<"--------SMART HOME LOG------------"<<std::endl;
    mujDum.Simulace(8,50,15,"zkouska.log");
    return 0;
}
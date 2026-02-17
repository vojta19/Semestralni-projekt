#include <iostream>
#include <string>

class Sensor
{
private:
    std::string mistnost;
    std::string typ;
    double hodnota;

public:
    Sensor(std::string m,std::string t,double startHodnota);
    void zmerit();

    std::string getMistnost();
    std::string getTyp();
    double getHodnota();
};
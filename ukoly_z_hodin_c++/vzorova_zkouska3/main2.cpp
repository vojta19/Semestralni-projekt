#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include "logger2.h"

int main()
{
    srand(time(0));
    Logger2 mujLogger2 ("log2.txt");
    Server mujServer(&mujLogger2);
    mujServer.run(50);
    return 0;
}
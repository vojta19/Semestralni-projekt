#include "Student.h"
#include <sstream>

Student::Student(){
    
}

Student::Student(std::string radek){
    std::stringstream ss(radek);
    std::string idStr, nameStr, prumerStr;

    std::getline(ss, idStr, ',');
    std::getline(ss, nameStr, ',');
    std::getline(ss, prumerStr, ',');

    id = stoi(idStr);
    name = nameStr;
    prumer = stod(prumerStr);
}

std::ostream& operator<<(std::ostream& out, Student& m)
{
    out << m.id << ", " << m.name << ", " << m.prumer;
    return out;
}
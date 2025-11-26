#pragma once

#include <string>
#include "Node.h"


class LinkedList
{
private:
    Node **pHead;
    std::string filename;

    Student parsujRadek(const std::string &radek);
    void ulozStudentyDoSouboru();

public:
    LinkedList(const std::string &filename);
    void vypisSeznam();
    void pridejStudentaNaKonec(Student new_Student);
    // void setridSeznamPodlePrumeru();
    ~LinkedList();
    friend std::ostream &operator<<(std::ostream &out, const LinkedList &m);
};
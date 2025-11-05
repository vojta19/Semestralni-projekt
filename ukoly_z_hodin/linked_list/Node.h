#pragma once

#include <string>

// definuje strukturu Student
struct Student
{
    int id;
    std::string name;
    double prumer;
};
// definuje strukturu Node

class Node
{
private:
    Student student;
    Node *next;

public:
    Node();
    Node(Student newStudent);
    Node* get_next();
    Student get_student();
    void set_next(Node* next);
    void vypis_node();
};
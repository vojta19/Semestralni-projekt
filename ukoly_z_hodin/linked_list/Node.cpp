#include "Node.h"
#include <iostream>
#include <iomanip>

Node::Node()
{
    student = Student();
    next = nullptr;
}
Node::Node(Student newStudent)
{
    student = newStudent;
    next = nullptr;
}
Node* Node::get_next()
{
    return next;
}

Student Node::get_student()
{
    return student;
}

void Node::set_next(Node* next)
{
    this->next = next;
}

void Node::vypis_node()
{
    std::cout << student.id << std::setw(5) << student.name << std::setw(5) << student.prumer << '\n';
}

// Node *operator[](int index)
// {
//     Node *current = this;
//     for (int i = 0; i < index; i++)
//     {
//         if (current == nullptr)
//         {
//             return nullptr;
//         }
//         current = current->next;
//     }
//     return current;
// }
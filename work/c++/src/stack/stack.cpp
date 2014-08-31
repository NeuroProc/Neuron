/*
* STACK REALIZATION BY ARTYOM4iK
*/
#include <iostream>
#include "stack.h"

void stack::initialize() {
    head = 0;   
}

void stack::push(void *data) {
    note *tmp = new note;

    tmp->data = data;
    tmp->next = head;
    head = tmp;
}

void* stack::pop() {

    if (head == 0)
        return 0;
    
    note *tmp = head;
    void *result = head->data;

    head = head->next;
    delete tmp;

    return result;
}

void stack::cleanup() {
    if (head != 0)
        std::cout << "ERROR: STACK NOT CLEAN!";
}








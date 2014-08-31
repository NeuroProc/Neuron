/*
* STACK HEADER BY ARTYOM4iK
*/


#ifndef STACK_H
#define STACK_H

struct stack {
    struct note {
        void *data;
        note *next;
    } *head;
    void initialize();
    void push(void *);
    void *pop();
    void cleanup();
};

#endif

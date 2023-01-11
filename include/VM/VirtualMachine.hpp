#ifndef _VIRTUAL_MACHINE_HPP
#define _VIRTUAL_MACHINE_HPP

#include "../Stack/Stack.hpp"

class VM
{
    private:
        Stack *memory = nullptr;
    public:
        //'mem_allocated' refers to how many blocks will be created on the stack.
        VM(const int &mem_allocated = 0);
        ~VM();



};


#endif
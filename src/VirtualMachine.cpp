#include "../include/VM/VirtualMachine.hpp"

VM::VM(const int &mem_allocated)
{
    std::cout << "Initializing Virtual Machine...\n" << std::endl;

    if (mem_allocated == 0)
    {
        this->memory = new Stack;
    }
    else
    {
        this->memory = new Stack{mem_allocated};
    }

    if (!this->memory->isStackInitialized())
    {
        std::cerr << "VIRTUAL MACHINE ERROR: FAILED TO INITIALIZE STACK" << std::endl;
        return;
    }

    std::cout << "Virtual Machine initialized succesfully!\nVirtual Machine is ready to use!" << std::endl;

}

VM::~VM()
{
    delete memory;
}



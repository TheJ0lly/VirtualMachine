#include "../include/Stack/Stack.hpp"
#include "../include/VM/VirtualMachine.hpp"

using namespace std;

//This file is used for running the Virtual Machine as a program, which runs until the user quits.

int main()
{
    Stack mem;

    std::cout << mem.getStackValue() << std::endl;
    std::cout << mem.getStackValueType() << std::endl;
}
#include "../include/Stack/Stack.hpp"
#include "../include/VM/VirtualMachine.hpp"

using namespace std;

//This file is used for running the Virtual Machine as a program, which runs until the user quits.

int main()
{

    //Example of how to load a string into the STACK.
    std::vector<uint16_t> currentProgram;
    Stack mem;

    //We init the string we want to store.
    std::string temp{"Matei"};

    //We set the starting point of the string.
    currentProgram.push_back(mem.getCurrentLocation());

    //We put all the characters into the memory, block after block starting from the first memory block.
    for (std::string::size_type i = 0; i < temp.size(); i++)
    {
        mem.changeValueAtCurrentLocation('s', 'c', temp[i]);
        mem.moveStackPointer(mem.getCurrentLocation() + 1);
    }

    //After the last block containg a character, we put the terminator.
    mem.changeValueAtCurrentLocation('s', 't');

    //We move the Stack Pointer to the beginning of the string in memory.
    mem.moveStackPointer(currentProgram[0]);

    //We look for the value of the terminator.
    while (mem.getStackValue() != -1)
    {
        if (mem.getStackValueType() == 'c')
        {
            std::cout << (char)mem.getStackValue();
        }
        else if (mem.getStackValueType() == 'i')
        {
            std::cout << mem.getStackValue();
        }
        else
        {
            break;
        }

        mem.moveStackPointer(mem.getCurrentLocation() + 1);
        
        //If we don't find a terminator, that means we forgot to add it, thus we access every block of memory. In this if statement we check if it hits the last block of the stack, in that case, we HALT.
        if (mem.getCurrentLocation() == mem.getEOS())
        {
            std::cout << "\nSTOP" << std::endl;
            break;
        }
    }

    std::cout << std::endl;


    

    /*
    
    Example of the language

    x = "test"       looks to allocate 4 characters in the stack next to each other
    output(x)       will output the string starting from the address of 'x'


    */
}
#ifndef _STACK_HPP
#define _STACK_HPP

#include <vector>
#include <iostream>

#define STACK_MEMORY_MAX 65535

//Return values
#define STACK_OPERATION_SUCCES 0

//Errors

//Error indicating that the user is accesing a block of memory out of the capacity of the current stack.
#define OUT_OF_STACK_MEMORY -1
//Error indicating that a block of memory which is used for storage, is valued as a program, which is not correct.
#define PROGRAM_TYPE_WHERE_STORAGE -2
//Error indicating that a block of memory which is used for a program, is valued as a variable, which is not correct.
#define VARIABLE_TYPE_WHERE_PROGRAM -3




class Stack
{
    private:
        
        /*
            Block represents a block of memory from inside the Stack.
                'value' refers to the value inside the block.
                'type' refers to the type of block: storage or program.
                'dataType' refers to the type of the value in the block, or the return type of a program.
        */
        typedef struct 
        {
            uint16_t value;
            char type;
            char dataType;
        } Block;


        std::vector<Block> memory;
        uint16_t END_OF_STACK;
        bool stackInitialized = false;
        
        uint16_t StackPointer;


    public:
        
        //Initializes the memory stack. 'memory_allocated' refers to how many blocks of 32 bits to be constructed.
        Stack(const int &memory_allocated = STACK_MEMORY_MAX);
        ~Stack();

        //Moves the stack pointer to a specific address in the stack. Returns STACK_OPERATION_SUCCES(0) on succes, otherwise either PROTECTED_MEMORY_HIT(-1) or OUT_OF_STACK_MEMORY(-2).
        uint16_t moveStackPointer(const uint16_t &location);

        //Returns the value from the stack at the address of 'StackPointer'.
        uint16_t getStackValue();

        //Returns the address of the current block.
        uint16_t getCurrentLocation();

        //Changes the value of the block at the current address.
        void changeValueAtCurrentLocation(const uint16_t &value, const char &type, const char &dataType);

        //Returns 'true' if the stack has been initialized successfully, else 'false'.
        bool isStackInitialized();

        //Outputs information about the stack.
        void getStackInfo();
};


#endif
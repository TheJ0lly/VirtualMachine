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
//Error indicating that a value is exceeding 32 bits.
#define OUT_OF_RANGE_ASSIGNMENT -4
//Error indicating unknown block type
#define UNKNOWN_BLOCK_TYPE -5
//Error indicating unknown data type
#define UNKNOWN_DATA_TYPE -6



class Stack
{
    private:
        
        /*
            Block represents a block of memory from inside the Stack. A block of memory consist of 3 values adding up to 48 bits/6 bytes.
                'value' refers to the value inside the block.
                'dataType' refers to the type of the value in the block, or the return type of a program.
        */
        typedef struct Block
        {

            union
            {
                int IValue;
                float FValue;
                char CValue;
            };
            
            char dataType;
            Block(): dataType('n') {}; 
        } Block;


        std::vector<Block> memory;
        uint16_t END_OF_STACK;
        bool stackInitialized = false;
        
        uint16_t StackPointer;


    public:
        
        Stack(const int &memory_allocated = STACK_MEMORY_MAX);
        ~Stack();

        uint16_t moveStackPointer(const uint16_t &location);

        float getStackValue_FLOAT(const int &location = -1);

        int getStackValue_INT(const int &location = -1);

        char getStackValue_CHAR(const int &location = -1);

        char getStackValueType(const int &location = -1);

        uint16_t getCurrentLocation();

        uint16_t changeValueAtCurrentLocation(const char &dataType = 'n', const float &value = 0);

        bool isStackInitialized();

        void getStackInfo();

        uint16_t getEOS();
};


#endif
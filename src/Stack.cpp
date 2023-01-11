#include "../include/Stack/Stack.hpp"


Stack::Stack(const int &memory_allocated)
{    
    //Checking if the memory allocated by the user is not exceeding the limits of the stack. 
    if (memory_allocated > STACK_MEMORY_MAX)
    {
        std::cerr << "STACK ERROR: MEMORY TO ALLOCATE EXCEEDS THE STACK LIMIT" << std::endl;
        return;
    }

    //We allocate 'memory allocated' worth of blocks of 32 bits.
    try
    {
        this->memory.reserve(memory_allocated);
    }
    catch(const std::length_error &ex)
    {
        std::cerr << "STACK ERROR: " << ex.what() << std::endl;
        return;
    }
    catch(const std::bad_alloc &ex)
    {
        std::cerr << "STACK ERROR: " << ex.what() << std::endl;
        return;
    }

    //We announce the user that the allocation of memory has been successful and we give him proper messages to explain what is happening at the moment.
    std::cout << "Stack has been allocated: " << memory.capacity() << " blocks." << std::endl;
    std::cout << "Initializing stack memory..." << std::endl;
    
    //We intialize the 'END_OF_STACK' as the upper limit.
    this->END_OF_STACK = memory.capacity() - 1;

    std::cout << "Blocks available to use: " << memory.capacity() << ".\nAddresses available: (0-" << END_OF_STACK << ")." << std::endl;
    std::cout << "Memory allocated: " << sizeof(Block) * memory_allocated << " bytes.\nSize of one block is: " << sizeof(Block) << std::endl;
    this->StackPointer = 0;
    std::cout << "Stack pointer pointing to block 0 in memory." << std::endl;
    
    std::cout << "Memory initialization complete!\n" << std::endl;
    this->stackInitialized = true;

}

Stack::~Stack()
{

}

uint16_t Stack::moveStackPointer(const uint16_t &location)
{
    if (location > this->END_OF_STACK)
    {
        std::cerr << "STACK ERROR: OUT OF STACK MEMORY BLOCK" << std::endl;
        return OUT_OF_STACK_MEMORY;
    }

    this->StackPointer = location;
    return STACK_OPERATION_SUCCES;
}

uint16_t Stack::getStackValue()
{
    return this->memory[this->StackPointer].value;
}

uint16_t Stack::getCurrentLocation()
{
    return this->StackPointer;
}

void Stack::changeValueAtCurrentLocation(const uint16_t &value, const char &type, const char &dataType)
{    
    this->memory[this->StackPointer].value = value;
    this->memory[this->StackPointer].type = type;
    this->memory[this->StackPointer].dataType = dataType;
}

bool Stack::isStackInitialized()
{
    return this->stackInitialized;
}

void Stack::getStackInfo()
{
    std::cout << "Blocks of memory: " << memory.capacity() << std::endl;
    std::cout << "Size of one block: " << sizeof(Block) << std::endl;
    std::cout << "Bytes used by the stack: " << sizeof(Block) * memory.capacity() << std::endl;
}
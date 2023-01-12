#include "../include/Stack/Stack.hpp"


//Initializes the memory stack. 'memory_allocated' refers to how many blocks of 32 bits to be constructed.
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
    std::cout << "Memory allocated: " << sizeof(Block) * memory_allocated << " bytes.\nSize of one block is: " << sizeof(Block) << " bytes." << std::endl;
    this->StackPointer = 0;
    std::cout << "Stack pointer pointing to block 0 in memory." << std::endl;
    
    std::cout << "Memory initialization complete!\n" << std::endl;
    this->stackInitialized = true;

}

Stack::~Stack()
{

}

//Moves the stack pointer to a specific address in the stack. Returns STACK_OPERATION_SUCCES(0) on succes, otherwise either PROTECTED_MEMORY_HIT(-1) or OUT_OF_STACK_MEMORY(-2).
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

//Returns the value from the stack at the address of 'StackPointer'.
int16_t Stack::getStackValue(const int &location)
{
    if (location == -1)
    {
        return this->memory[this->StackPointer].value;
    }
    else if (location > this->END_OF_STACK)
    {
        std::cerr << "STACK ERROR: ACCESSING OUT OF STACK MEMORY BLOCK" << std::endl;
        return OUT_OF_STACK_MEMORY;
    }
    return this->memory[location].value;
}

//Returns the address of the current block.
uint16_t Stack::getCurrentLocation()
{
    return this->StackPointer;
}
        
/*
    Changes the values of the block at the current address. 
    'value' refers to the value inside the block. Can be anything between 0 - 65535.
    'blockType' refers to the type of the block. Can be 's' or 'p'.
    'dataType' refers to the type of the value. Can be 'i', 'c' or 't'
*/
uint16_t Stack::changeValueAtCurrentLocation(const char &blockType, const char &dataType, const int16_t &value)
{
    if (value > UINT16_MAX)
    {
        std::cerr << "STACK ERROR: VALUE ASSIGNMENT EXCEEDS LIMIT OF 16 BITS" << std::endl;
        return OUT_OF_RANGE_ASSIGNMENT;
    }
    if (blockType != 's' && blockType != 'p')
    {
        std::cerr << "STACK ERROR: BLOCK TYPE ASSIGNMENT IS UNKNOWN\nTypes of blocks supported:\n\t-Storage\t's'\n\t-Program\t'p'" << std::endl;
        return UNKNOWN_BLOCK_TYPE;
    }
    if (dataType != 'i' && dataType != 'c' && dataType != 't')
    {
        std::cerr << "STACK ERROR: VALUE DATATYPE ASSIGNMENT IS UNKNOWN\nTypes of data types supported:\n\t-Integer\t'i'\n\t-Character\t'c'\n\t-Terminator\t't'" << std::endl;
        return UNKNOWN_DATA_TYPE;
    }

    if (dataType == 't')
    {
        this->memory[this->StackPointer].value = -1;
    }
    else
    {
        this->memory[this->StackPointer].value = value;
    }
    
    this->memory[this->StackPointer].blockType = blockType;
    this->memory[this->StackPointer].dataType = dataType;

    return STACK_OPERATION_SUCCES;
}

//Returns 'true' if the stack has been initialized successfully, else 'false'.
bool Stack::isStackInitialized()
{
    return this->stackInitialized;
}

//Outputs information about the stack.
void Stack::getStackInfo()
{
    std::cout << "Blocks of memory: " << memory.capacity() << std::endl;
    std::cout << "Size of one block: " << sizeof(Block) << std::endl;
    std::cout << "Bytes used by the stack: " << sizeof(Block) * memory.capacity() << std::endl;
}

//Returns the last address of the stack.
uint16_t Stack::getEOS()
{
    return this->END_OF_STACK;
}
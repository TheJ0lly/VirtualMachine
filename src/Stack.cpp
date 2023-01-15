#include "../include/Stack/Stack.hpp"


//Initializes the memory stack. 'memory_allocated' refers to how many blocks of 48 bits to be constructed.
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

    for (int i = 0; i <= this->END_OF_STACK; i++)
    {
        this->memory.push_back(Block());
    }


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

//Moves the stack pointer to a specific address in the stack. Returns STACK_OPERATION_SUCCES(0) on succes, otherwise OUT_OF_STACK_MEMORY(-2).
uint16_t Stack::moveStackPointer(const int32_t &location)
{
    if (location > this->END_OF_STACK)
    {
        std::cerr << "STACK ERROR: OUT OF STACK MEMORY BLOCK" << std::endl;
        return OUT_OF_STACK_MEMORY;
    }

    this->StackPointer = location;
    return STACK_OPERATION_SUCCES;
}

float Stack::getStackValue_FLOAT(const int &location)
{
    if (location == -1)
    {
        return this->memory[this->StackPointer].FValue;
    }
    else if (location > this->END_OF_STACK)
    {
        std::cerr << "STACK ERROR: ACCESSING OUT OF STACK MEMORY BLOCK" << std::endl;
        return OUT_OF_STACK_MEMORY;
    }
    return this->memory[location].FValue;
}

int Stack::getStackValue_INT(const int &location)
{
    if (location == -1)
    {
        return this->memory[this->StackPointer].IValue;
    }
    else if (location > this->END_OF_STACK)
    {
        std::cerr << "STACK ERROR: ACCESSING OUT OF STACK MEMORY BLOCK" << std::endl;
        return OUT_OF_STACK_MEMORY;
    }
    return this->memory[location].IValue;
}

char Stack::getStackValue_CHAR(const int &location)
{
    if (location == -1)
    {
        return this->memory[this->StackPointer].CValue;
    }
    else if (location > this->END_OF_STACK)
    {
        std::cerr << "STACK ERROR: ACCESSING OUT OF STACK MEMORY BLOCK" << std::endl;
        return OUT_OF_STACK_MEMORY;
    }
    return this->memory[location].CValue;
}

//Returns the data type of the value in the block.
char Stack::getStackValueType(const int &location)
{
    if (location == -1)
    {
        return this->memory[this->StackPointer].dataType;
    }
    else if (location > this->END_OF_STACK)
    {
        std::cerr << "STACK ERROR: ACCESSING OUT OF STACK MEMORY BLOCK" << std::endl;
        return OUT_OF_STACK_MEMORY;
    }

    return this->memory[location].dataType;
}


//Returns the address of the current block.
uint32_t Stack::getCurrentLocation()
{
    return this->StackPointer;
}
        
/*
    Changes the values of the block at the current address.
    'value' refers to the value inside the block. -1 if is terminator.
    'dataType' refers to the type of the value. Can be 'i', 'c', 'f' or 't'.
*/
uint16_t Stack::changeValueAtCurrentLocation(const char &dataType, const float &value)
{

    if (dataType == 'n')
    {
        this->memory[this->StackPointer].dataType = 'n';
        return STACK_OPERATION_SUCCES;
    }
    else
    {
        if (dataType != 'i' && dataType != 'c' && dataType != 't' && dataType != 'f')
        {
            std::cerr << "STACK ERROR: VALUE DATATYPE ASSIGNMENT IS UNKNOWN\nTypes of data types supported:\n\t-Integer\t'i'\n\t-Character\t'c'\n\t-Terminator\t't'\n\t-Float\t\t'f'" << std::endl;
            return UNKNOWN_DATA_TYPE;
        }
    }


    if (value > INT32_MAX)
    {
        std::cerr << "STACK ERROR: VALUE ASSIGNMENT EXCEEDS LIMIT OF 32 BITS" << std::endl;
        return OUT_OF_RANGE_ASSIGNMENT;
    }
    
    this->memory[this->StackPointer].dataType = dataType;

    if (dataType == 'c')
    {
        this->memory[this->StackPointer].CValue = (char)value;
    }
    else if (dataType == 'f')
    {
        this->memory[this->StackPointer].FValue = value;
    }    
    else if (dataType == 'i')
    {
        this->memory[this->StackPointer].IValue = (int)value;
    }    
    else if (dataType == 't')
    {
        this->memory[this->StackPointer].IValue = -1;
    }

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
uint32_t Stack::getEOS()
{
    return this->END_OF_STACK;
}
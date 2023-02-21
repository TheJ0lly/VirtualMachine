#include "../include/VM/VirtualMachine.hpp"

//'mem_allocated' refers to how many blocks will be created on the stack.
VM::VM(const int &mem_allocated)
{
    std::cout << "========== INTIALIZATION ==========" << std::endl;
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
    std::cout << "=====================================\n" << std::endl;

}

VM::~VM()
{
    delete memory;
}

VAR_PAIR VM::storeString(const std::string &value)
{
    this->memory->moveStackPointer(0);
    int len = (int)value.length();
    int checkLen = 0;
    int loc = 0;
    int start = 0;


    while (true)
    {
        if (this->memory->getStackValueType(loc) == 'n')
        {
            checkLen++;
        }
        else
        {
            checkLen = 0;
            start = loc + 1;
        }

        loc++;
        if (this->memory->moveStackPointer(loc) != STACK_OPERATION_SUCCES)
        {
            std::cerr << "VM ERROR: CANNOT ALLOCATE MEMORY FOR THE STRING: " << value << std::endl;
            return VAR_PAIR{VM_FAILED_TO_ALLOCATE, 'n'};
        }

        if (checkLen == len)
        {
            for (int i = 0; i < len; i++)
            {
                this->storeChar(value[i], start+i);
            }
            this->memory->moveStackPointer(start + len);
            this->memory->changeValueAtCurrentLocation('t');
            return VAR_PAIR{start, 's'};
        }
    }
    

}

VAR_PAIR VM::storeChar(const char &value, const int &location)
{
    int result;
    if ( (result = this->memory->moveStackPointer(location)) == OUT_OF_STACK_MEMORY)
    {
        std::cerr << "VM ERROR: MEMORY LOCATION INVALID" << std::endl;
        return VAR_PAIR{VM_FAILED_TO_STORE_DATA, 'n'};
    }

    if ( (result = this->memory->changeValueAtCurrentLocation('c', value) ) != STACK_OPERATION_SUCCES )
    {
        std::cerr << "VM ERROR: FAILED TO STORE CHAR" << std::endl;
        return VAR_PAIR{result, 'n'};
    }
    return VAR_PAIR{location, 'c'};
    
}

VAR_PAIR VM::storeFloat(const float &value, const int &location)
{
    int result;
    if ( (result = this->memory->moveStackPointer(location)) == OUT_OF_STACK_MEMORY)
    {
        std::cerr << "VM ERROR: MEMORY LOCATION INVALID" << std::endl;
        return VAR_PAIR{VM_FAILED_TO_STORE_DATA, 'n'};
    }

    if ( (result = this->memory->changeValueAtCurrentLocation('f', value) ) != STACK_OPERATION_SUCCES )
    {
        std::cerr << "VM ERROR: FAILED TO STORE FLOAT" << std::endl;
        return VAR_PAIR{result, 'n'};
    }
    return VAR_PAIR{location, 'f'};
    
}

VAR_PAIR VM::storeInt(const int &value, const int &location)
{
    int result;
    if (this->memory->moveStackPointer(location) == OUT_OF_STACK_MEMORY)
    {
        std::cerr << "VM ERROR: MEMORY LOCATION INVALID" << std::endl;
        return VAR_PAIR{VM_FAILED_TO_STORE_DATA, 'n'};
    }

    if (this->memory->changeValueAtCurrentLocation('i', value) != STACK_OPERATION_SUCCES )
    {
        std::cerr << "VM ERROR: FAILED TO STORE INT" << std::endl;
        return VAR_PAIR{result, 'n'};
    }
    return VAR_PAIR{location, 'i'};
    
}

char VM::retrieveChar(const int &location)
{
    if (this->memory->moveStackPointer(location) == OUT_OF_STACK_MEMORY)
    {
        return VM_FAILED_TO_RETREIVE;
    }

    if (this->memory->getStackValueType() != 'c' && this->memory->getStackValueType() != 't')
    {
        std::cerr << "VM ERROR: CANNOT RETRIEVE CHAR FROM BLOCK WITH ANOTHER DATA TYPE" << std::endl;
        return VM_FAILED_TO_RETREIVE;
    }

    return this->memory->getStackValue_CHAR(location);

}

float VM::retrieveFloat(const int &location)
{
    if (this->memory->moveStackPointer(location) == OUT_OF_STACK_MEMORY)
    {
        return VM_FAILED_TO_RETREIVE;
    }

    if (this->memory->getStackValueType() != 'f' && this->memory->getStackValueType() != 't')
    {
        std::cerr << "VM ERROR: CANNOT RETRIEVE FLOAT FROM BLOCK WITH ANOTHER DATA TYPE" << std::endl;
        return VM_FAILED_TO_RETREIVE;
    }

    return this->memory->getStackValue_FLOAT(location);

}

int VM::retrieveInt(const int &location)
{
    if (this->memory->moveStackPointer(location) == OUT_OF_STACK_MEMORY)
    {
        return VM_FAILED_TO_RETREIVE;
    }

    if (this->memory->getStackValueType() != 'i' && this->memory->getStackValueType() != 't')
    {
        std::cerr << "VM ERROR: CANNOT RETRIEVE INT FROM BLOCK WITH ANOTHER DATA TYPE" << std::endl;
        return VM_FAILED_TO_RETREIVE;
    }

    return this->memory->getStackValue_INT(location);

}

uint32_t VM::getStackPointerLocation()
{
    return this->memory->getCurrentLocation();
}

void VM::getVMInfo()
{
    std::cout << "======== INFORMATION ========" << std::endl;
    this->memory->getStackInfo();
    std::cout << "VM version: 1.0" << std::endl;
    std::cout << "=============================" << std::endl;
}
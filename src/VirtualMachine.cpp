#include "../include/VM/VirtualMachine.hpp"


//============================================================PUBLIC===============================================================
//'mem_allocated' refers to how many blocks will be created on the stack.

//================= VM SPECIFIC =================

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

//===============================================




//================== VARIABLES ==================

int VM::storeChar(const char &value, VAR &var)
{

    if (var.second != 'n')
    {
        this->memory->moveStackPointer(var.first);
        this->memory->changeValueAtCurrentLocation('n');
        this->freeVariable(var);
    }

    for (uint32_t i = 0; i <= this->memory->getEOS(); i++)
    {
        if (this->memory->getStackValueType(i) == 'n')
        {
            this->memory->moveStackPointer(i);
            this->memory->changeValueAtCurrentLocation('c', value);
            var.first = i;
            var.second = 'c';
            return VM_SUCCES;
        }
    }

    std::cerr << "VIRTUAL MACHINE: FAILED TO ALLOCATE MEMORY FOR CHAR" << std::endl;
    return VM_FAILED_STORE_CHAR;
}

int VM::storeFloat(const float &value, VAR &var)
{
    if (var.second != 'n')
    {
        this->memory->moveStackPointer(var.first);
        this->memory->changeValueAtCurrentLocation('n');
        this->freeVariable(var);
    }

    for (uint32_t i = 0; i <= this->memory->getEOS(); i++)
    {
        if (this->memory->getStackValueType(i) == 'n')
        {
            this->memory->moveStackPointer(i);
            this->memory->changeValueAtCurrentLocation('f', value);
            var.first = i;
            var.second = 'f';
            return VM_SUCCES;
        }
    }

    std::cerr << "VIRTUAL MACHINE: FAILED TO ALLOCATE MEMORY FOR FLOAT" << std::endl;
    return VM_FAILED_STORE_FLOAT;    
}

int VM::storeInt(const int &value, VAR &var)
{
    if (var.second != 'n')
    {
        this->memory->moveStackPointer(var.first);
        this->memory->changeValueAtCurrentLocation('n');
        this->freeVariable(var);
    }

    for (uint32_t i = 0; i <= this->memory->getEOS(); i++)
    {
        if (this->memory->getStackValueType(i) == 'n')
        {
            this->memory->moveStackPointer(i);
            this->memory->changeValueAtCurrentLocation('i', value);
            var.first = i;
            var.second = 'i';
            return VM_SUCCES;
        }
    }

    std::cerr << "VIRTUAL MACHINE: FAILED TO ALLOCATE MEMORY FOR INT" << std::endl;
    return VM_FAILED_STORE_INT;    
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

void VM::freeVariable(VAR &var)
{
    var.first = -1;
    var.second = 'n';
}

//===============================================




//==================== ARRAY ====================

int VM::appendChar(const char &value, ARRAY &arr)
{
    VAR temp;
    temp.first = -1;
    temp.second = 'n';
    if (this->storeChar(value, temp) == VM_FAILED_STORE_CHAR )
    {
        std::cerr << "VM ERROR: CANNOT ALLOCATE SPACE FOR CHAR" << std::endl;
        return VM_FAILED_APPEND_CHAR;
    }

    arr.push_back(temp);
    return VM_SUCCES;
}

int VM::appendInt(const int &value, ARRAY &arr)
{
    VAR temp;
    temp.first = -1;
    temp.second = 'n';
    if (this->storeInt(value, temp) == VM_FAILED_STORE_INT)
    {
        std::cerr << "VM ERROR: CANNOT ALLOCATE SPACE FOR INT" << std::endl;
        return VM_FAILED_APPEND_INT;
    }

    arr.push_back(temp);
    return VM_SUCCES;

}

int VM::appendFloat(const float &value, ARRAY &arr)
{
    VAR temp;
    temp.first = -1;
    temp.second = 'n';
    if (this->storeFloat(value, temp) == VM_FAILED_STORE_FLOAT)
    {
        std::cerr << "VM ERROR: CANNOT ALLOCATE SPACE FOR FLOAT" << std::endl;
        return VM_FAILED_APPEND_FLOAT;
    }

    arr.push_back(temp);
    return VM_SUCCES;
}

int VM::removeElement(const int &location, ARRAY &arr)
{
    if (location < 0)
    {
        std::cerr << "ARRAY ERROR: INDEX BELOW 0" << std::endl;
        return VM_FAILED_REMOVE;
    }

    if (location > (int)arr.size() - 1)
    {
        std::cerr << "ARRAY ERROR: EXCEEDING SIZE OF THE ARRAY" << std::endl;
        return VM_FAILED_REMOVE;
    }

    arr.erase(arr.begin() + location);
    return VM_SUCCES;


}

int VM::at(const int &index, ARRAY &arr, VAR &var)
{
    if (index < 0)
    {
        std::cerr << "ARRAY ERROR: INDEX BELOW 0" << std::endl;
        return VM_FAILED_AT;
    }

    if (index > (int)arr.size() - 1)
    {
        std::cerr << "ARRAY ERROR: EXCEEDING SIZE OF THE ARRAY" << std::endl;
        return VM_FAILED_AT;
    }

    var.first = arr[index].first;
    var.second = arr[index].second;
    return VM_SUCCES;
}

void VM::clearArray(ARRAY &arr)
{
    arr.clear();
}

//===============================================




//===============================================================Standard Functions Available on the VM===============================================================

int VM::print(const VAR &var, bool endline)
{
    switch(var.second)
    {
        case 'c':
            if (endline)
                std::cout << this->retrieveChar(var.first) << std::endl;
            else
                std::cout << this->retrieveChar(var.first);
            break;
        case 'f':
            if (endline)
                std::cout << this->retrieveFloat(var.first) << std::endl;
            else
                std::cout << this->retrieveFloat(var.first);
            break;
        case 'i':
            if (endline)
                std::cout << this->retrieveInt(var.first) << std::endl;
            else
                std::cout << this->retrieveInt(var.first);
            break;
        default:
            std::cerr << "VARIABLE ERROR: VARIABLE HAS UNKNOWN TYPE" << std::endl;
            return PRINT_UNKNOWN_TYPE;
    }
    return STDLIB_SUCCESS;
}

int VM::print(const ARRAY &arr, bool asString)
{
    if (!asString)
    {
        std::cout << "ARRAY: ";
        for (int i = 0; i < (int)arr.size(); i++)
        {
            char type = this->memory->getStackValueType(arr[i].first);
            switch(type)
            {
                case 'i':
                    std::cout << this->retrieveInt(arr[i].first);
                    break;
                case 'f':
                    std::cout << this->retrieveFloat(arr[i].first);
                    break;            
                case 'c':
                    std::cout << "'" <<this->retrieveChar(arr[i].first) << "'";
                    break;
                default:
                    std::cerr << "VM ERROR: ACCESSING BLOCK WITH UNKNOWN DATA TYPE" << std::endl;
                    return VM_UNKNOWN_DATA_TYPE;
            }

            if (i != (int)arr.size() - 1)
            {
                std::cout << ", ";
            }
            else
            {
                std::cout << std::endl;
            }
        }
    }
    else
    {
        std::cout << "STRING: ";
        for (int i = 0; i < (int)arr.size(); i++)
        {
            char type = this->memory->getStackValueType(arr[i].first);
            switch(type)
            {
                case 'i':
                    std::cout << this->retrieveInt(arr[i].first);
                    break;
                case 'f':
                    std::cout << this->retrieveFloat(arr[i].first);
                    break;            
                case 'c':
                    std::cout << this->retrieveChar(arr[i].first);
                    break;
                default:
                    std::cerr << "VM ERROR: ACCESSING BLOCK WITH UNKNOWN DATA TYPE" << std::endl;
                    return VM_UNKNOWN_DATA_TYPE;
            }

            if (i == (int)arr.size() - 1)
            {
                std::cout << std::endl;
            }
        }
    }


    
    return VM_SUCCES;
}

int VM::print(const std::string &string, bool endline)
{
    std::cout << string;
    if (endline)
        std::cout << std::endl;
    return VM_SUCCES;
}
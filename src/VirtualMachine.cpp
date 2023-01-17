#include "../include/VM/VirtualMachine.hpp"

//'mem_allocated' refers to how many blocks will be created on the stack.
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

    std::cout << "Virtual Machine initialized succesfully!\nVirtual Machine is ready to use!\n" << std::endl;

}

VM::~VM()
{
    delete memory;
}


int VM::storeData(const char &dataType, const float &value)
{
    int result = this->memory->changeValueAtCurrentLocation(dataType, value);

    if (result == UNKNOWN_DATA_TYPE || result == OUT_OF_RANGE_ASSIGNMENT)
    {
        return VM_FAILED_TO_STORE_DATA;
    }

    return VM_SUCCES;
}

RetrievedData VM::retrieveData(const int &location)
{
    RetrievedData result;
    result.type = this->memory->getStackValueType(location);
    
    switch (result.type)
    {
        case 'i':
            result.IVALUE = this->memory->getStackValue_INT(location);
            return result;
        case 'c':
            result.CVALUE = this->memory->getStackValue_CHAR(location);
            return result;
        case 'f':
            result.FVALUE = this->memory->getStackValue_FLOAT(location);
            return result;
        default:
            std::cerr << "VIRTUAL MACHINE ERROR: FAILED TO RETRIEVE DATA" << std::endl;
            result.type = 'n';
            result.IVALUE = VM_FAILED_TO_RETREIVE;
            return result;
    }
}

/*
    'value' indicates the string read from file or from user to interpret
    'type' indicates to the data type of the value. Can be 'i', 'f' or 's'.
*/
std::pair<int, char> VM::allocateMemory(const std::string &value, const char &type)
{
    int i;
    float f;
    switch (type)
    {
        case 'i':
        {
            i = std::stoi(value.c_str());
            for (int x = 0; x < STACK_MEMORY_MAX; x++)
            {
                this->memory->moveStackPointer(x);
                if (this->memory->getStackValueType() == 'n')
                {
                    this->memory->changeValueAtCurrentLocation('i', i);
                    return std::pair<int, char>{x, 'i'};
                }
            }
            std::cerr << "VIRTUAL MACHINE ERROR: FAILED TO ALLOCATE MEMORY FOR INTEGER: " << i << std::endl;
            return std::pair<int, char>{VM_FAILED_TO_ALLOCATE, 'i'};
        }
        case 'f':
        {
            f = std::stof(value.c_str());
            for (int x = 0; x < STACK_MEMORY_MAX; x++)
            {
                this->memory->moveStackPointer(x);
                if (this->memory->getStackValueType() == 'n')
                {
                    this->memory->changeValueAtCurrentLocation('f', f);
                    return std::pair<int, char>{x, 'f'};
                }
            }
            std::cerr << "VIRTUAL MACHINE ERROR: FAILED TO ALLOCATE MEMORY FOR FLOAT: " << f << std::endl;
            return std::pair<int, char>{VM_FAILED_TO_ALLOCATE, 'f'};
        }

        case 's':
        {
            int ok = 0;
            int len = value.size();
            int start = 0;
            int check = 0;
            int counter = 0;
            for (int x = 0; x < STACK_MEMORY_MAX; x++)
            {
                this->memory->moveStackPointer(x);
                if (check == len)
                {
                    ok = 1;
                    break;
                }
                
                if (this->memory->getStackValueType() != 'n')
                {
                    check = 0;
                    start = x + 1;
                    continue;
                }
                else
                {
                    check++;
                }
                
            }

            if (ok)
            {
                for (int x = start; x < start + len; x++)
                {
                    this->memory->moveStackPointer(x);
                    this->memory->changeValueAtCurrentLocation('c', value[counter]);
                    counter++;
                }
                this->memory->moveStackPointer(this->memory->getCurrentLocation() + 1);
                this->memory->changeValueAtCurrentLocation('t');
                return std::pair<int, char>{start, 's'};
            }
            else
            {
                std::cerr << "VIRTUAL MACHINE ERROR: FAILED TO ALLOCATE MEMORY FOR STRING: " << value << std::endl;
                return std::pair<int, char>{VM_FAILED_TO_ALLOCATE, 's'};
            }
        }
        default:
            std::cerr << "VIRTUAL MACHINE ERROR: UNKNOWN DATA TYPE" << std::endl;
            return std::pair<int, char>{VM_UNKNOWN_DATA_TYPE, 'n'};
    }
}


int VM::deallocateMemory(Var &var)
{
    int add = var.address;
    int len = 0;

    if (var.type == 's')
    {
        for (int i = var.address; i < STACK_MEMORY_MAX; i++)
        {
            this->memory->moveStackPointer(i);
            if (this->memory->getStackValueType() != 'n' && this->memory->getStackValueType() != 't')
            {
                len++;
            }
            else
            {
                break;
            }
        }

        for (int i = 0; i < len + 1; i++)
        {
            this->memory->moveStackPointer(add + i);
            this->memory->changeValueAtCurrentLocation('n');
        }

        var.address = -1;
        var.type = 'n';
        return VM_SUCCES;
    }
    this->memory->moveStackPointer(var.address);
    this->memory->changeValueAtCurrentLocation('n');
    var.address = -1;
    var.type = 'n';
    return VM_SUCCES;

    
}


std::pair<std::vector<int>, char> VM::allocateMemoryArray(const std::vector<std::string> &values, const char &type)
{
    int len = values.size();
    int check = 0;
    int start = 0;
    int counter = 0;
    std::vector<int> addresses;

    switch(type)
    {
        case 'i':
        {
            for (int x = 0; x < STACK_MEMORY_MAX; x++)
            {
                if (check == len)
                {
                    break;
                }


                if (this->memory->getStackValueType() != 'n')
                {
                    check = 0;
                    start = x + 1;
                    continue;
                }
                else
                {
                    check++;
                }
            }

            for (int x = start; x < len + start; x++)
            {
                Var temp = this->allocateMemory(values[counter], type);
                counter++;
                addresses.push_back(temp.address);
            }

            return std::pair<std::vector<int>, char>{addresses, 'i'};

        }
        case 'f':
        {
            for (int x = 0; x < STACK_MEMORY_MAX; x++)
            {
                if (check == len)
                {
                    break;
                }


                if (this->memory->getStackValueType() != 'n')
                {
                    check = 0;
                    start = x + 1;
                    continue;
                }
                else
                {
                    check++;
                }
            }

            for (int x = start; x < len + start; x++)
            {
                Var temp = this->allocateMemory(values[counter], type);
                counter++;
                addresses.push_back(temp.address);
            }

            return std::pair<std::vector<int>, char>{addresses, 'f'};
        }
        case 's':
        {
            for (int x = 0; x < STACK_MEMORY_MAX; x++)
            {
                if (check == len)
                {
                    break;
                }

                if (this->memory->getStackValueType() != 'n')
                {
                    check = 0;
                    start = x + 1;
                    continue;
                }
                else
                {
                    check++;
                }
            }
            
            for (int x = start; x < len + start; x++)
            {
                Var temp = this->allocateMemory(values[counter], type);
                counter++;
                addresses.push_back(temp.address);
            }
            return std::pair<std::vector<int>, char>{addresses, 's'};
        }
    }
}

int VM::deallocateMemoryArray(Array &arr)
{
    int size = arr.addresses.size();

    switch (arr.type)
    {
        case 'i':
            for (int i = size - 1; i >= 0; i--)
            {
                this->memory->moveStackPointer(arr.at(i).address);
                this->memory->changeValueAtCurrentLocation('n', -1);
                arr.addresses.pop_back();
            }
            break;
        case 'f':
            for (int i = size - 1; i >= 0; i--)
            {
                this->memory->moveStackPointer(arr.at(i).address);
                this->memory->changeValueAtCurrentLocation('n', -1);
                arr.addresses.pop_back();
            }
            break;
        case 's':
            for (int i = size - 1; i >= 0; i--)
            {
                int add = arr.at(i).address;
                int len = 0;

                this->memory->moveStackPointer(arr.at(i).address);
                for (int x = arr.at(i).address; x < STACK_MEMORY_MAX; x++)
                {
                    this->memory->moveStackPointer(x);
                    if (this->memory->getStackValueType() != 'n' && this->memory->getStackValueType() != 't')
                    {
                        len++;
                    }
                    else
                    {
                        break;
                    }
                }

                for (int x = 0; x <= len; x++)
                {
                    this->memory->moveStackPointer(add + i);
                    this->memory->changeValueAtCurrentLocation('n');
                }
                arr.addresses.pop_back();
            }
            break;
    }

    return VM_SUCCES;
}

int VM::print(const Var &variable)
{
    if (variable.type == 'n')
    {
        std::cerr << "VIRTUAL MACHINE ERROR: TRIED PRINTING UNUSED MEMORY" << std::endl;
        return VM_TRIED_ACCESSING_UNUSED_BLOCK;
    }

    switch (variable.type)
    {
        case 'i':
            std::cout << this->memory->getStackValue_INT(variable.address) << std::endl;
            return VM_SUCCES;
        case 'f':
            std::cout << this->memory->getStackValue_FLOAT(variable.address) << std::endl;
            return VM_SUCCES;
        case 's':
        {
            int counter = variable.address;
            while (this->memory->getStackValueType(counter) != 't')
            {
                std::cout << this->memory->getStackValue_CHAR(counter);
                counter++;
            }
            std::cout << std::endl;
            return VM_SUCCES;
        }
        default:
            if (variable.type != 'i' && variable.type != 'f' && variable.type != 's')
            {
                std::cerr << "VIRTUAL MACHINE ERROR: UNKNOWN DATA TYPE FOR VARIABLE" << std::endl;
                return VM_UNKNOWN_DATA_TYPE;
            }
    }
    return VM_SUCCES;
}

//TODO
// int VM::print(const std::string &variable)
// {
    
// }

int VM::print(const int &location)
{
    char type = this->memory->getStackValueType(location);

    switch(type)
    {
        case 'i':
            std::cout << this->memory->getStackValue_INT(location) << std::endl; 
            break;
        case 'f':
            std::cout << this->memory->getStackValue_FLOAT(location) << std::endl;
            break;
        case 'c':
            std::cout << this->memory->getStackValue_CHAR(location) << std::endl;
            break;
    }
    return VM_SUCCES;
}
#ifndef _VIRTUAL_MACHINE_HPP
#define _VIRTUAL_MACHINE_HPP

#include "../Stack/Stack.hpp"

//Errors
#define VM_SUCCES               			0
#define VM_UNKNOWN_DATA_TYPE    			-1
#define VM_FAILED_TO_STORE_DATA 			-2
#define VM_FAILED_TO_RETREIVE               -3
#define VM_FAILED_TO_ALLOCATE               -4
#define VM_TRIED_ACCESSING_UNUSED_BLOCK		-5


typedef struct RetrievedData
{
    union
    {
        int IVALUE;
        float FVALUE;
        char CVALUE;
    };
    char type;

    friend std::ostream &operator<<(std::ostream &out, RetrievedData &retDat)
    {
        switch (retDat.type)
        {
            case 'i':
                std::cout << retDat.IVALUE;
                return out;
            case 'f':
                std::cout << retDat.FVALUE;
                return out;
            case 'c':
                std::cout << retDat.CVALUE;
                return out;
            default:
                std::cout << "NONE";
                return out;
        }
    };
    RetrievedData(): type('n') {};

} RetrievedData;


typedef struct Variable
{
    int address;
    char type;
    Variable(): address(-1), type('n') {};
    Variable(const std::pair<int, char> &var): address(var.first), type(var.second) {};

} Var;

typedef struct Array
{
    std::vector<int> addresses;
    char type;
    Array(): type('n'){};
    Array(const std::pair<std::vector<int>, char> &array): addresses(array.first), type(array.second){};
    Var at(const uint32_t &location)
    {
        if (location >= addresses.size())
        {
            std::cerr << "ARRAY ACCESSING ERROR: TRYING TO ACCESS UNALLOCATED MEMORY" << std::endl;
            return Var{std::pair<int, char>{-1, 'n'}};
        }
        return Var{std::pair<int, char>{this->addresses[location], type}};
    }
} Array;

//MAX MEMORY THAT CAN BE ALLOCATED TO THE VM IS: 1_000_000
class VM
{
    private:

        Stack *memory = nullptr;
    public:
        VM(const int &mem_allocated = 0);
        ~VM();


        int storeData(const char &dataType, const float &value);
        RetrievedData retrieveData(const int &location = -1);

        std::pair<int, char> allocateMemory(const std::string &value, const char &type);
        int deallocateMemory(Var &var);


        std::pair<std::vector<int>, char> allocateMemoryArray(const std::vector<std::string> &values, const char &type);
        int deallocateMemoryArray(Array &arr);


        int print(const Var &variable);
        int print(const std::string &variable);
        int print(const int &location);

};


#endif
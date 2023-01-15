#ifndef _VIRTUAL_MACHINE_HPP
#define _VIRTUAL_MACHINE_HPP

#include "../Stack/Stack.hpp"

//Errors
#define VM_SUCCES               0
#define VM_UNKNOWN_DATA_TYPE    -1
#define VM_FAILED_TO_STORE_DATA -2
#define VM_FAILED_TO_RETREIVE   -3
#define VM_FAILED_TO_ALLOCATE   -4


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

class VM
{
    friend Var;
    private:

        Stack *memory = nullptr;
    public:
        VM(const int &mem_allocated = 0);
        ~VM();


        int storeData(const char &dataType, const float &value);
        RetrievedData retrieveData(const int &location = -1);

        std::pair<int, char> allocateMemory(const std::string &value, const char &type);
        int deallocateMemory(Var &var);


        void print(const Var &variable);
        void print(const std::string &variable);

};


#endif
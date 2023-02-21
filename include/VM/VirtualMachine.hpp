#ifndef _VIRTUAL_MACHINE_HPP
#define _VIRTUAL_MACHINE_HPP

#include "../Stack/Stack.hpp"

//Errors
#define VM_SUCCES               			 0
#define VM_UNKNOWN_DATA_TYPE    			-1
#define VM_FAILED_TO_STORE_DATA 			-2
#define VM_FAILED_TO_RETREIVE               -3
#define VM_FAILED_TO_ALLOCATE               -4
#define VM_TRIED_ACCESSING_UNUSED_BLOCK		-5
#define VM_NO_MEMORY_ALLOCATED              -6
#define VM_FILE_NOT_FOUND                   -7
#define VM_NO_FILE_GIVEN                    -8
#define VM_DIRECTORY_INSTEAD_FILE           -9
#define VM_NOT_REGULAR_FILE                 -10
#define VM_NOT_SUPPORTED_FILE               -11

//MAX MEMORY THAT CAN BE ALLOCATED TO THE VM IS: 1_000_000


typedef std::pair<int, char> VAR_PAIR;
class VM
{
    private:
        
        Stack *memory = nullptr;
    public:
        VM(const int &mem_allocated = 0);
        ~VM();

        VAR_PAIR storeString(const std::string &value);
        VAR_PAIR storeChar(const char &value, const int &location = -1);
        VAR_PAIR storeFloat(const float &value, const int &location = -1);
        VAR_PAIR storeInt(const int &value, const int &location = -1);

        char retrieveChar(const int &location = -1);
        float retrieveFloat(const int &location = -1);
        int retrieveInt(const int &location = -1);

        uint32_t getStackPointerLocation();
        void getVMInfo();

};



#endif
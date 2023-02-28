#ifndef _VIRTUAL_MACHINE_HPP
#define _VIRTUAL_MACHINE_HPP

#include "../Stack/Stack.hpp"
#include <vector>


//Errors
#define VM_SUCCES               			 0
#define VM_UNKNOWN_DATA_TYPE    			-1
#define VM_FAILED_TO_STORE_DATA 			-2
#define VM_FAILED_TO_RETREIVE               -3
#define VM_FAILED_TO_ALLOCATE               -4

#define VM_FAILED_STORE_CHAR                -5
#define VM_FAILED_STORE_INT                 -6
#define VM_FAILED_STORE_FLOAT               -7

#define VM_FAILED_APPEND_CHAR               -8
#define VM_FAILED_APPEND_INT                -9
#define VM_FAILED_APPEND_FLOAT              -10

#define VM_FAILED_AT                        -11

//STDLIB Errors
#define STDLIB_SUCCESS                       0
#define PRINT_UNKNOWN_TYPE                  -1

//One-Value Variable
typedef std::pair<int, char> VAR;

//Multiple-Value Variable
typedef std::vector<VAR> ARRAY;

class VM
{
    private:
        Stack *memory = nullptr;
    public:

        //================= VM SPECIFIC =================
        
        VM(const int &mem_allocated = 0);
        ~VM();

        uint32_t getStackPointerLocation();
        void getVMInfo();

        //===============================================


        //================== VARIABLES ==================

        int storeChar(const char &value, VAR &var);
        int storeFloat(const float &value, VAR &var);
        int storeInt(const int &value, VAR &var);

        char retrieveChar(const int &location = -1);
        float retrieveFloat(const int &location = -1);
        int retrieveInt(const int &location = -1);

        void deallocateVariable(VAR &var);

        //===============================================


        //==================== ARRAY ====================

        int appendChar(const char &value, ARRAY &arr);
        int appendInt(const int &value, ARRAY &arr);
        int appendFloat(const float &value, ARRAY &arr);

        int at(ARRAY &arr, VAR &var, const int &index);
        void clearArray(ARRAY &arr);

        //===============================================

    
        //Standard Functions Available on the VM
        
        int print(const VAR &var, bool endline = true);
        int print(const ARRAY &arr);
        int print(const std::string &var, bool endline = true);



};



#endif
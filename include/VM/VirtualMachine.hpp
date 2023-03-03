#ifndef _VIRTUAL_MACHINE_HPP
#define _VIRTUAL_MACHINE_HPP

#include "../Stack/Stack.hpp"
#include <vector>

#define NAME_OF(var) #var

//Errors
#define VM_SUCCES               			 0
#define VM_UNKNOWN_DATA_TYPE    			-1
#define VM_FAILED_TO_STORE_DATA 			-2
#define VM_FAILED_TO_RETREIVE               -3
#define VM_FAILED_TO_ALLOCATE               -4

//Variables

#define VM_FAILED_STORE_CHAR                -5
#define VM_FAILED_STORE_INT                 -6
#define VM_FAILED_STORE_FLOAT               -7

//Arrays

#define VM_FAILED_APPEND_CHAR               -8
#define VM_FAILED_APPEND_INT                -9
#define VM_FAILED_APPEND_FLOAT              -10

#define VM_FAILED_AT                        -11
#define VM_FAILED_REMOVE                    -12

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

        //Stores a 'char' and assigns the data type and location in memory to the 'var'
        int storeChar(const char &value, VAR &var);

        //Stores a 'float' and assigns the data type and location in memory to the 'var'
        int storeFloat(const float &value, VAR &var);

        //Stores a 'int' and assigns the data type and location in memory to the 'var'
        int storeInt(const int &value, VAR &var);

        //Retrieves a 'char' from the selected location in memory
        char retrieveChar(const int &location = -1);

        //Retrieves a 'float' from the selected location in memory
        float retrieveFloat(const int &location = -1);

        //Retrieves a 'int' from the selected location in memory
        int retrieveInt(const int &location = -1);

        //Makes the variable 'free'. No data type and no location to point to
        void freeVariable(VAR &var);

        //===============================================


        //==================== ARRAY ====================

        //Adds a 'char' to the end of an array 
        int appendChar(const char &value, ARRAY &arr);

        //Adds a 'int' to the end of an array 
        int appendInt(const int &value, ARRAY &arr);

        //Adds a 'float' to the end of an array 
        int appendFloat(const float &value, ARRAY &arr);

        //Removes an element from the specified location in array
        int removeElement(const int &index, ARRAY &arr);

        //Returns a 'VAR' from the position 'index' of the array
        int at(const int &index, ARRAY &arr, VAR &var);

        //Clears all elements from inside the array
        void clearArray(ARRAY &arr);

        //===============================================

    
        //Standard Functions Available on the VM
        
        int print(const VAR &var, bool endline = true);
        int print(const ARRAY &arr, bool asString = false);
        int print(const std::string &var, bool endline = true);



};



#endif
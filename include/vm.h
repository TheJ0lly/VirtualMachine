#ifndef VM_H_
#define VM_H_

#include <stdint.h>
#include <stdbool.h>

#include "vm_errors.h"

#define DEFAULT_MEM_SPACE UINT16_MAX
#define DEFAULT_INST_STARTPOINT 0xC350
#define DEFAULT_DATA_STARTPOINT 0x0

// Memory layout of an instruction
/*
4 bits - OP code
12 bits - parameters
*/
typedef enum {
    /*
    3 bits - Register
    9 bits - Value
    */
    MV = 0,

    /*
    3 bits - Register
    9 bits - Memory location
    */
    LD,

    /*
    3 bits - Register
    9 bits - Memory location
    */
    ST,
} Operations;

typedef enum {
    /*
    I/O register:
        - all I/O operations will store the characters in R0.
    */
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    REGISTER_LEN,

    /* 
    Compare flag:
        - 1, if the value on the right is bigger.
        - 0, if the values are equal.
        - -1, if the value on the left is bigger.
    */
    RBool,
} Register;

typedef struct {
    uint16_t *memory;
    uint16_t *reg;
    
    uint16_t ip;

    /*
    INST_START:
        - is a constant that is used to store 
          the first memory location of the instruction space.
    */
    uint16_t inst_start;

    uint16_t dp;

    bool running;
} VM;

Error init_vm(VM *vm, uint16_t memory, uint16_t inst_startpoint, uint16_t data_startpoint);

Error mem_write(VM *vm, uint16_t addr, uint16_t val);
Error mem_read(VM *vm, uint16_t addr, uint16_t *reg);

Error vm_execute_instruction(VM *vm, uint16_t inst);


#endif
#ifndef VM_H_
#define VM_H_

#include <stdint.h>
#include <stdbool.h>

#include "vm_errors.h"

#define DEFAULT_MEM_SPACE UINT16_MAX
#define DEFAULT_IP_STARTPOINT 0xC350
#define DEFAULT_DATA_STARTPOINT 0x0

#define DEBUG


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
    LDI,

    /*
    3 bits - Register
    9 bits - Memory location
    */
    ST,
    STI,

    /*
    3 bits - Reg
    3 bits - Reg
    */
    CMP,

    /*
    12 bits - Memory location
    */
    JMP,
    JMPI,

    /*
    Nothing. It stops the vm.
    */
    HALT,
} Operations;

#define OP_MV(reg, val) (((MV << 3) | (reg)) << 9) | (val)
#define OP_LD(reg, loc) (((LD << 3) | (reg)) << 9) | (loc)
#define OP_LDI(reg, loc) (((LDI << 3) | (reg)) << 9) | (loc)
#define OP_ST(reg, loc) (((ST << 3) | (reg)) << 9) | (loc)
#define OP_STI(reg, loc) (((STI << 3) | (reg)) << 9) | (loc)
#define OP_CMP(reg1, reg2) ((((CMP << 3) | (reg1) ) << 3) | (reg2)) << 6 
#define OP_HALT (HALT << 12)

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
        - 0, if the values are equal.
        - 1, if the value on the left is bigger.
        - 2, if the value on the right is bigger.
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
    uint16_t ip_start;

    uint16_t dp;

    bool running;
} VM;

void dbg_print_all_instructions(VM *vm);
void dbg_print_memory(VM *vm);
void dbg_print_registers(VM *vm);

void dbg_print_instruction(VM *vm, uint16_t inst);

Error init_vm(VM *vm, uint16_t memory, uint16_t inst_startpoint, uint16_t data_startpoint);

Error mem_write(VM *vm, uint16_t addr, uint16_t val, bool direct);
Error mem_read(VM *vm, uint16_t addr, uint16_t *reg, bool direct);

Error vm_execute_instruction(VM *vm, uint16_t inst);

Error vm_load_program(VM *vm, uint16_t *program, uint16_t icount);
Error vm_execute_program(VM *vm);


#endif
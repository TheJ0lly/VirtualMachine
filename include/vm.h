#ifndef VM_H_
#define VM_H_

#include <stdint.h>
#include <stdbool.h>

#include "vm_errors.h"

#define DEFAULT_MEM_SPACE UINT16_MAX
#define DEFAULT_IP_STARTPOINT 0x0
#define DEFAULT_DATA_STARTPOINT 0x1000


typedef enum {
    MV = 0,
    LD,
    LDI,
    ST,
    STI,
    CMP,
    JMP,
    HALT,
} Operation;

typedef struct Instruction {
    Operation op;
    uint8_t reg1;
    uint8_t reg2;
    uint16_t loc;
} Instruction;

#define OP_MV(reg, val) {.op=MV, .reg1=(reg), .reg2=(val)}
#define OP_LD(reg, lo) {.op=LD, .reg1=(reg), .loc=(lo)}
#define OP_LDI(reg, lo) {.op=LDI, .reg1=(reg), .loc=(lo)}
#define OP_ST(reg, lo) {.op=ST, .reg1=(reg), .loc=(lo)}
#define OP_STI(reg, lo) {.op=STI, .reg1=(reg), .loc=(lo)}
#define OP_CMP(r1, r2) {.op=CMP, .reg1=(r1), .reg2=(r2)}
#define OP_JMP(lo) {.op=MV, .loc=(lo)}
#define OP_HALT {.op=HALT}

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
    Instruction *insts;
    uint16_t insts_cap;
    uint16_t *memory;
    uint16_t mem_cap;
    uint16_t *reg;
    
    uint16_t ip;

    bool running;
} VM;

void dbg_print_all_instructions(VM *vm);
void dbg_print_memory(VM *vm);
void dbg_print_registers(VM *vm);

void dbg_print_instruction(VM *vm, Instruction inst);

Error init_vm(VM *vm, uint16_t memory);

Error mem_write(VM *vm, uint16_t addr, uint16_t val, bool direct);
Error mem_read(VM *vm, uint16_t addr, uint16_t *reg, bool direct);

Error vm_execute_instruction(VM *vm, Instruction inst);
Error vm_load_program(VM *vm, Instruction *program, uint16_t icount);
Error vm_execute_program(VM *vm);


#endif
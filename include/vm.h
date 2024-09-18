#ifndef VM_H_
#define VM_H_

#include <stdint.h>
#include <stdbool.h>

#include "vm_errors.h"

#define DEFAULT_MEM_SPACE UINT16_MAX
#define DEFAULT_START_POINT 0xC350

typedef struct {
    /*
    I/O register:
        - all I/O operations will store the characters in R0.
    */
    uint16_t R0;
    uint16_t R1;
    uint16_t R2;
    uint16_t R3;
    uint16_t R4;
    uint16_t R5;
    uint16_t R6;
    uint16_t R7;

    /* 
    Compare flag:
        - 1, if the value on the right is bigger.
        - 0, if the values are equal.
        - -1, if the value on the left is bigger.
    */
    uint8_t RBool;
} Register;

typedef struct {
    uint16_t *memory;
    
    uint16_t ip;

    uint16_t inst_start;
    
    Register reg;

    bool running;
} VM;

Error init_vm(VM *vm, uint16_t memory, uint16_t startpoint);

Error mem_write(VM *vm, uint16_t addr, uint16_t val);
Error mem_read(VM *vm, uint16_t addr, uint16_t *reg);

#endif
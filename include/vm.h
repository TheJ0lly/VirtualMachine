#ifndef _VM_H_
#define _VM_H_

#include <stdint.h>

/*
16 bit virtual machine

Instruction:
    0 0 0 0 | 0 0 ... 0 0
    Op Code | at least 4 are parameters.

    Meaning, an instruction may have, 1 byte(8 bits) 2 bytes(16 bits) or 4 bytes(32 bits)

Memory layout:
    Addr
    16 bits
        0             1    
        Lower bits    Higher bits

    etc.
*/

enum Register {
    R0 = 0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    RCOMP,

    /* 
    Used for getting the number of registers. 
    DO NOT USE AS REGISTER.
    */
    RLEN,
};

extern const uint16_t default_ip_start;
extern const uint16_t max_memory_allowed;

struct machine {
    uint8_t *memory;
    uint16_t reg[RLEN];
    uint16_t ip;
};

/*
    Machine functions
*/

// Create a new virtual machine `memory` amount of bytes.
void new_machine(struct machine *vm, uint16_t memory);


/* 
    Memory handling functions.
*/

// Read 1 byte located at addr. 
uint8_t read(struct machine *vm, uint16_t addr);

// Write 1 byte at addr.
void write(struct machine *vm, uint16_t addr, uint8_t val);

// Read 2 bytes starting from addr.
uint16_t read2(struct machine *vm, uint16_t addr);

// Write 2 bytes starting at addr.
void write2(struct machine *vm, uint16_t addr, uint16_t val);



#endif
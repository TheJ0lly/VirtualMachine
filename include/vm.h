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

Stack:
    Push and Pop move the stack pointer.
    The rest of operations DO NOT affect the stack pointer, ONLY the instruction pointer.

    0x0 - Stack pointer start
    0x1
    0x2
    0x3
    .
    .
    .

    The offset from the default starting position is the number of elements on the stack.
    Stack pointer = 0x0 - 0 elements.
    Stack pointer = 0x1 - 1 element.
    .
    .
    .
*/

enum Error {
    OK = 0,
    EMPTY_STACK,

};

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
    RERROR,
    /* 
    Used for getting the number of registers. 
    DO NOT USE AS REGISTER.
    */
    RLEN,
};

extern const uint16_t default_ip_start;
extern const uint16_t max_memory_allowed;
extern const uint16_t default_sp_start;

// The structure for the virtual machine.
struct machine {
    // The memory, holding at most 65535(UINT16_MAX) bytes.
    uint8_t *memory;

    // The registers, that can be accessed with the Register enum.
    uint16_t reg[RLEN];

    // Instruction pointer, which points to the next instruction to execute. DEFAULT AT 50000
    uint16_t ip;

    // Stack pointer, which points to the top of the stack memory. DEFAULT AT 40000.
    uint16_t sp;
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


/*
    Stack handling functions.
*/

// Push a 16-bit value on top of the stack.
void push(struct machine *vm, uint16_t val);

// Pop the top of the stack 16-bit value.
uint16_t pop(struct machine *vm);



#endif
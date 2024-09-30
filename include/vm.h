#ifndef _VM_H_
#define _VM_H_

#include <stdint.h>
#include <stdbool.h>

/*
16 bit virtual machine


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
    UNKNOWN_OP,
    EMPTY_STACK,
    STACK_OVERFLOW,
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

    /*
    1 - left side is bigger.
    0 - equal.
    2 - right side is bigger.
    */
    RCOMP,
    RERROR,
    /* 
    Used for getting the number of registers. 
    DO NOT USE AS REGISTER.
    */
    RLEN,
};

extern const uint16_t default_ip_start;
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

    bool running;
};

/*
    Machine functions
*/

// Create a new virtual machine `memory` amount of bytes.
void new_machine(struct machine *vm);


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
    Instructions.

    Instruction:
    0 0 0 0 0 0 0 0 | 0 0 0 0 0 0 0 0 ... 0 0 0 0 0 0 0 0
        Op Code     | 24 bits for parameters.

    Meaning, an instruction has 28 bits of parameters and each occupies at most 4 bytes.

*/

#define GET_BYTE_1(x) (uint8_t)(((x) & 0xFF000000) >> 24)
#define GET_BYTE_2(x) (uint8_t)(((x) & 0x00FF0000) >> 16)
#define GET_BYTE_3(x) (uint8_t)(((x) & 0x0000FF00) >> 8)
#define GET_BYTE_4(x) (uint8_t)(((x) & 0x000000FF))

enum Operation {
    /*
        20 bit - signals if the value is 8 or 16 bits.
            1 - 16 bits.
            
        24 bit - signals if the value comes from a register.
               - 1 a register
        23-21 bits - which register
            OR
        16 - 1 bits - the value
    */
    PUSH = 0,

    /*
        24 bit - signals if the value will go into a register.
            - 1 a register
        23-21 bits - which register
        20 bit - signals if the value is 8 or 16 bits.
            1 - 16 bits.
    */
    POP,

    /*
        20 - 18 the register where to move it to.

        24 bit - will signal that the value will come from a register.
            - 1 a register
        23-21 bits - which register.
            OR
        16-1 bits - the value
    */
    MOV,

    /*
        24-22 bits - will signal which register.
    */
    INC,

    /*
        24-22 bits - will signal which register.
    */
    DEC,

    /*
        24-22 bits - will signal which register.
    */
    NEG,

    /*
        24-22 bits - will signal which register to add into.

        20 bit     - will signal that we will fetch a value from another register.
        19-17 bits - which register to get the value from.
            OR
        16-1 bits - the value to add.
    */
    ADD,

    /*
        24-22 bits - will signal which register to substract into.
        
        20 bit     - will signal that we will fetch a value from another register.
        19-17 bits - which register to get the value from.
            OR
        16-1 bits - the value to substract.
    */
    SUB,

    /*
        24-22 bits - will signal which register to substract into.
        
        20 bit     - will signal that we will fetch a value from another register.
        19-17 bits - which register to get the value from.
            OR
        16-1 bits - the value to substract.
    */
    MUL,

    /*
        24-22 bits - will signal which register to substract into.
        
        20 bit     - will signal that we will fetch a value from another register.
        19-17 bits - which register to get the value from.
            OR
        16-1 bits - the value to substract.
    */
    DIV,

    /*
        24-21 bits:
            0 - Input
            1 - Output

        20-17 bits:
            0 - 1 byte
            1 - 2 bytes
            2 - 4 bytes
            3 - string

        16-1 bits - where to (store/read from).
    */
    INT,

    /*
        24-22 bits - left register used for comparison.

        21-19 bits - right register used for comparison.
    */
    CMP,

    /*
        FOR ALL JUMPS.
        16-1 bits - where to jump to.
    */
    JMP,
    JGR,
    JGE,
    JLS,
    JLE,

    /*
        No parameters. Only halt.
    */
    HALT,
};

void load_instruction(struct machine *vm, uint32_t instruction);
uint32_t fetch_next_instruction(struct machine *vm);
void execute_instruction(struct machine *vm, uint32_t instruction);
void execute_program(struct machine *vm);


#endif
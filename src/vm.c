#include "../include/vm.h"
#include <malloc.h>

/*
    Constants
*/
const uint16_t default_ip_start = 0xC350;       //50000
const uint16_t max_memory_allowed = UINT16_MAX; //65535
const uint16_t default_sp_start = 0x9C40;       //40000

void new_machine(struct machine *vm, uint16_t memory) {
    vm->memory = calloc(memory, sizeof(uint8_t));
    vm->ip = default_ip_start;
    vm->sp = default_sp_start;
    vm->running = true;

    /*
    We set it to 0, because errors will set this flag to a non-zero value.
    */
    vm->reg[RERROR] = 0;
}

uint8_t read(struct machine *vm, uint16_t addr) {
    return vm->memory[addr];
}

uint16_t read2(struct machine *vm, uint16_t addr) {
    return ((uint16_t)vm->memory[addr]) | (((uint16_t)vm->memory[addr+1]) << 8);
}

void write(struct machine *vm, uint16_t addr, uint8_t val) {
    vm->memory[addr] = val;
}

void write2(struct machine *vm, uint16_t addr, uint16_t val) {
    vm->memory[addr] = (uint8_t)(val & 0x00FF);
    vm->memory[addr + 1] = (uint8_t)(val >> 8);
}

void load_instruction(struct machine *vm, uint32_t instruction) {
    enum Operation op = (instruction & 0xF0000000) >> 28;

    switch (op) {
    case PUSH:
    {
        uint8_t first_byte = GET_BYTE_1(instruction);
        uint8_t second_byte = GET_BYTE_3(instruction);
        uint8_t third_byte = GET_BYTE_4(instruction);
        vm->memory[vm->ip++] = first_byte;
        vm->memory[vm->ip++] = second_byte;
        vm->memory[vm->ip++] = third_byte;
    }
    break;
    
    case POP:
    {
        uint8_t first_byte = GET_BYTE_1(instruction);
        vm->memory[vm->ip++] = first_byte;
    }
    break;

    case HALT:
    {
        uint8_t first_byte = GET_BYTE_1(instruction);
        vm->memory[vm->ip++] = first_byte; 
    }
    break;

    default:
        printf("unknown op: %d\n", op);
        break; 
    }
}

void push(struct machine *vm, uint16_t val) {
    if (vm->sp + 1 == default_ip_start) {
        vm->reg[RERROR] = STACK_OVERFLOW;
        fprintf(stderr, "cannot push - entering instructions section (addr: %d)\n", vm->sp + 1);
        return;
    }
    vm->memory[++vm->sp] = val;
}

uint16_t pop(struct machine *vm) {
    if (vm->sp == default_sp_start) {
        vm->reg[RERROR] = EMPTY_STACK;
        fprintf(stderr, "cannot pop - stack is empty (addr: %d)\n", vm->sp - 1);
        return 0;
    }
    return vm->memory[vm->sp--];
}
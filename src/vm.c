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
    enum Operation op = GET_BYTE_1(instruction);

    switch (op) {
    // 4 bytes instructions    
    case PUSH:
    case MOV:
    case ADD:
    case SUB:
    case MUL:
    case SMUL:
    case DIV:
    case SDIV:
    case INT:
        vm->memory[vm->ip++] = op;
        vm->memory[vm->ip++] = GET_BYTE_2(instruction);
        vm->memory[vm->ip++] = GET_BYTE_3(instruction);
        vm->memory[vm->ip++] = GET_BYTE_4(instruction);
        break;
    
    // Jumps
    case JMP:
    case JGR:
    case JGE:
    case JLS:
    case JLE:
        vm->memory[vm->ip++] = op;
        vm->memory[vm->ip++] = GET_BYTE_3(instruction);
        vm->memory[vm->ip++] = GET_BYTE_4(instruction);
        break;

    // 2 bytes instructions
    case POP:
    case INC:
    case DEC:
    case NEG:
    case CMP:
        vm->memory[vm->ip++] = op;
        vm->memory[vm->ip++] = GET_BYTE_2(instruction);
        break;

    // 1 byte instructions
    case HALT:
        vm->memory[vm->ip++] = op;
        break;

    default:
        printf("unknown op: %d\n", op);
        break; 
    }    
}

uint32_t fetch_next_instruction(struct machine *vm) {
    enum Operation op = read(vm, vm->ip++);
    uint32_t instruction = op;

    switch (op) {
    // 4 bytes instructions    
    case PUSH:
    case MOV:
    case ADD:
    case SUB:
    case MUL:
    case SMUL:
    case DIV:
    case SDIV:
    case INT:
        instruction = (instruction << 8) | read(vm, vm->ip++);
        instruction = (instruction << 8) | read(vm, vm->ip++);
        instruction = (instruction << 8) | read(vm, vm->ip++);
        break;
    
    // Jumps
    case JMP:
    case JGR:
    case JGE:
    case JLS:
    case JLE:
        instruction = instruction << 8; // Null space
        instruction = (instruction << 8) | read(vm, vm->ip++);
        instruction = (instruction << 8) | read(vm, vm->ip++);
        break;

    // 2 bytes instructions
    case POP:
    case INC:
    case DEC:
    case NEG:
    case CMP:
        instruction = (instruction << 8) | read(vm, vm->ip++);
        instruction = instruction << 16;
        break;

    // 1 byte instructions
    case HALT: // Nothing else to do.
        break;

    default:
        printf("unknown op: %d\n", op);
        break; 
    }

    return instruction;
}

void execute_instruction(struct machine *vm, uint32_t instruction) {
    enum Operation op = GET_BYTE_1(instruction);

    switch (op) {
    case PUSH:
        { 
            bool isreg = (instruction & 0x00800000) >> 23;
            bool is16 = (instruction & 0x00080000) >> 19;
            uint16_t value = (uint16_t)(instruction & 0x0000FFFF);

            if (isreg) {
                uint8_t reg = (instruction & 0x00700000) >> 20;
                value = vm->reg[reg];
            }

            // Create the offset
            vm->sp++;
            if (is16) {
                write2(vm, vm->sp, value);
                // We write 2 bytes, so we have a 2 bytes offset.
                vm->sp += 1;
            } else {
                write(vm, vm->sp, (uint8_t)(value & 0x00FF));
            }
        }
        break;
    default:
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
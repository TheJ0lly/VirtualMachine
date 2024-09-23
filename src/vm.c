#include "../include/vm.h"
#include <malloc.h>

/*
    Constants
*/
const uint16_t default_ip_start = 0xC350;
const uint16_t max_memory_allowed = UINT16_MAX;

void new_machine(struct machine *vm, uint16_t memory) {
    vm->memory = calloc(memory, sizeof(uint8_t));
    vm->ip = default_ip_start;
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

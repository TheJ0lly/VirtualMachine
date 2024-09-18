#include <malloc.h>

#include "../include/vm.h"


Error init_vm(VM *vm, uint16_t memory, uint16_t startpoint) {
    vm->ip = startpoint;
    vm->inst_start = startpoint;
    vm->running = true;

    vm->memory = calloc(memory, sizeof(uint16_t));

    if (vm->memory == NULL) {
        return ERR_FAILED_INIT_VM;
    }

    return ERR_OK;
}

Error mem_write(VM *vm, uint16_t addr, uint16_t val) {
    vm->memory[addr] = val;
    return ERR_OK;
}

Error mem_read(VM *vm, uint16_t addr, uint16_t *reg) {
    *reg = vm->memory[addr];
    return ERR_OK;
}
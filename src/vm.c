#include <malloc.h>

#include "../include/vm.h"


Error init_vm(VM *vm, uint16_t memory, uint16_t inst_startpoint, uint16_t stack_startpoint) {
    vm->ip = inst_startpoint;
    vm->inst_start = inst_startpoint;
    vm->dp = stack_startpoint;
    vm->running = true;


    vm->reg = calloc(REGISTER_LEN, sizeof(uint16_t));

    if (vm->reg == NULL) {
        return ERR_FAILED_INIT_VM;
    }

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

Error vm_execute_instruction(VM *vm, uint16_t inst) {
    uint16_t op = inst >> 12;

    switch (op) {
        case MV:
        {
            uint16_t reg = (inst >> 9) & 0x7;

            if (reg > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            uint16_t value = inst & 0x1FF;

            vm->reg[reg] = value;
        }
        break;
        
        case LD:
        {
            uint16_t reg = (inst >> 9) & 0x7;

            if (reg > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            uint16_t loc = inst & 0x1FF;

            if (loc >= vm->inst_start) {
                return ERR_INVALID_MEM_ACCESS;
            }

            mem_read(vm, loc, &vm->reg[reg]);
        }
        break;

        case ST:
        {
            uint16_t reg = (inst >> 9) & 0x7;

            if (reg > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            uint16_t loc = inst & 0x1FF;

            if (loc >= vm->inst_start) {
                return ERR_INVALID_MEM_ACCESS;
            }

            mem_write(vm, loc, vm->reg[reg]);
        }
        break;
        default:
            return ERR_UNKNOWN_OP;
    }
    return ERR_OK;
}
#include <malloc.h>
#include <memory.h>

#include "../include/vm.h"

void dbg_print_memory(VM *vm) {
    printf("===== VM MEMORY =====\n");
    uint16_t i = 0;
    while (i < vm->ip_start) {
        if (vm->memory[i] != 0) {
            printf("%hu: %hu\n", i, vm->memory[i]);
        }
        i++;
    }
}

void dbg_print_registers(VM *vm) {
    printf("===== VM REGISTERS =====\n");
    uint8_t i = 0;
    while (i < REGISTER_LEN) {
        printf("R%d: %d\n", i, vm->reg[i]);
        i++;
    }
}

void dbg_print_instruction(VM *vm, uint16_t inst) {
    uint16_t op = inst >> 12;
    uint16_t reg;
    uint16_t loc;

    switch (op) {
        case MV:
            reg = (inst >> 9) & 0x7;
            loc = inst & 0x1FF;
            printf("OP: MV\n   REG: %hu\n   VAL: %hu\n", reg, loc);
            break;
        case LD:
            reg = (inst >> 9) & 0x7;
            loc = inst & 0x1FF;
            printf("OP: LD\n   REG: %hu\n   LOC: %hu\n", reg, loc);
            break;
        case LDI:
            reg = (inst >> 9) & 0x7;
            loc = inst & 0x1FF;
            {
                uint16_t newloc = vm->memory[loc];
                uint16_t value = vm->memory[newloc];
                printf("OP: LDI\n   REG: %hu\n   LOC1: %hu\n   LOC2: %hu\n   VAL: %hu\n", reg, loc, newloc, value);
            }
            break;
        case ST:
            reg = (inst >> 9) & 0x7;
            loc = inst & 0x1FF;
            printf("OP: ST\n   REG: %hu\n   LOC: %hu\n", reg, loc);
            break;
        case STI:
            reg = (inst >> 9) & 0x7;
            loc = inst & 0x1FF;
            {
                uint16_t newloc = vm->memory[loc];
                printf("OP: STI\n   REG: %hu\n   LOC1: %hu\n   LOC2: %hu\n   REG_VAL: %hu\n", reg, loc, newloc, vm->reg[reg]);
            }
            break;
        case HALT:
            printf("OP: HALT\n");
            break;
        default:
            printf("UNKNOWN OPERATION: %hu\n", op);
            break;
    }

}

Error init_vm(VM *vm, uint16_t memory, uint16_t inst_startpoint, uint16_t stack_startpoint) {
    vm->ip = inst_startpoint;
    vm->ip_start = inst_startpoint;
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
    #ifdef DEBUG
    dbg_print_instruction(vm, inst);
    #endif

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

            if (loc >= vm->ip_start) {
                return ERR_INVALID_MEM_ACCESS;
            }

            mem_read(vm, loc, &vm->reg[reg]);
        }
        break;

        case LDI:
        {
            uint16_t reg = (inst >> 9) & 0x7;

            if (reg > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            uint16_t loc = inst & 0x1FF;

            if (loc >= vm->ip_start) {
                return ERR_INVALID_MEM_ACCESS;
            }

            uint16_t newloc;

            mem_read(vm, loc, &newloc);

            mem_read(vm, newloc, &vm->reg[reg]);
        }
        break;

        case ST:
        {
            uint16_t reg = (inst >> 9) & 0x7;

            if (reg > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            uint16_t loc = inst & 0x1FF;

            if (loc >= vm->ip_start) {
                return ERR_INVALID_MEM_ACCESS;
            }

            mem_write(vm, loc, vm->reg[reg]);
        }
        break;

        case STI:
        {
            uint16_t reg = (inst >> 9) & 0x7;

            if (reg > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            uint16_t loc = inst & 0x1FF;

            if (loc >= vm->ip_start) {
                return ERR_INVALID_MEM_ACCESS;
            }

            uint16_t newloc;

            mem_read(vm, loc, &newloc);

            mem_write(vm, newloc, vm->reg[reg]);
        }
        break;
        
        case HALT:
        vm->running = false;
        break;
        
        default:
            return ERR_UNKNOWN_OP;
    }
    return ERR_OK;
}

Error vm_load_program(VM *vm, uint16_t *program, uint16_t icount) {
    Error err;
    for (int i = 0; i < icount; i++) {
        err = mem_write(vm, vm->ip + i, program[i]);
        if (err != ERR_OK) {
            return err;
        }
    }
    return ERR_OK;
}

Error vm_execute_program(VM *vm) {
    #ifdef DEBUG
    printf("===== PROGRAM INSTRUCTIONS =====\n");
    #endif
    Error err;
    while (vm->running) {
        err = vm_execute_instruction(vm, vm->memory[vm->ip]);

        if (err != ERR_OK) {
            return err;
        }

        vm->ip++;
    }
    return ERR_OK;
}
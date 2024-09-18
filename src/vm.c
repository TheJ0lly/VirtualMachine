#include <malloc.h>
#include <memory.h>

#include "../include/vm.h"

void dbg_print_memory(VM *vm) {
    printf("===== VM MEMORY =====\n");
    uint16_t i = 0;
    while (i < vm->mem_cap) {
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
    printf("RBool: %d\n", vm->reg[RBool]);
}

void dbg_print_instruction(VM *vm, Instruction inst) {
    switch (inst.op) {
        case MV:
            printf("OP: MV\n   REG: %hu\n   VAL: %hu\n", inst.reg1, inst.reg2);
            break;
        case LD:
            printf("OP: LD\n   REG: %hu\n   LOC: %hu\n", inst.reg1, inst.loc);
            break;
        case LDI:
            {
                uint16_t newloc = vm->memory[inst.loc];
                uint16_t value = vm->memory[newloc];
                printf("OP: LDI\n   REG: %hu\n   LOC1: %hu\n   LOC2: %hu\n   VAL: %hu\n", inst.reg1, inst.loc, newloc, value);
            }
            break;
        case ST:
            printf("OP: ST\n   REG: %hu\n   LOC: %hu\n", inst.reg1, inst.loc);
            break;
        case STI:
            {
                uint16_t newloc = vm->memory[inst.loc];
                printf("OP: STI\n   REG: %hu\n   POINTER: %hu\n   DEST: %hu\n   REG_VAL: %hu\n", inst.reg1, inst.loc, newloc, vm->reg[inst.reg1]);
            }
            break;
        case CMP:
            {
                uint16_t r1 = vm->reg[inst.reg1];
                uint16_t r2 = vm->reg[inst.reg2];
                uint16_t rbool;
                if (r1 < r2)
                    rbool = 2;
                else if (r1 == r2)
                    rbool = 0;
                else
                    rbool = 1;

                printf("OP: CMP\n   REG1: %hu\n   REG2: %hu\n   CMP_VAL: %hu\n", r1, r2, rbool);
            }
            break;
        case JMP:
            {
                printf("OP: JMP\n   LOC: %hu\n", inst.loc);
            }
            break;
        case HALT:
            printf("OP: HALT\n");
            break;
        default:
            printf("UNKNOWN OPERATION: %hu\n", inst.op);
            break;
    }

}

Error init_vm(VM *vm, uint16_t memory) {
    vm->ip = DEFAULT_IP_STARTPOINT;
    vm->insts_cap = memory;
    vm->mem_cap = memory;
    vm->running = true;


    vm->reg = calloc(REGISTER_LEN, sizeof(uint16_t));

    if (vm->reg == NULL) {
        return ERR_FAILED_INIT_VM;
    }

    vm->memory = calloc(memory, sizeof(uint16_t));

    if (vm->memory == NULL) {
        return ERR_FAILED_INIT_VM;
    }

    vm->insts = calloc(memory, sizeof(Instruction));

    if (vm->insts == NULL) {
        return ERR_FAILED_INIT_VM;
    }

    return ERR_OK;
}

Error mem_write(VM *vm, uint16_t addr, uint16_t val, bool direct) {
    if (direct) {
        vm->memory[addr] = val;
    } else {
        vm->memory[vm->memory[addr]] = val;
    }
    return ERR_OK;
}

Error mem_read(VM *vm, uint16_t addr, uint16_t *reg, bool direct) {
    if (direct) {
        *reg = vm->memory[addr];
    } else {
        *reg = vm->memory[vm->memory[addr]];
    }
    return ERR_OK;
}

Error vm_execute_instruction(VM *vm, Instruction inst) {
    #ifdef DEBUG
    dbg_print_instruction(vm, inst);
    #endif

    switch (inst.op) {
        case MV:
        {
            if (inst.reg1 > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            vm->reg[inst.reg1] = inst.reg2;
        }
        break;
        
        case LD:
        {
            if (inst.reg1 > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            if (inst.loc >= vm->mem_cap) {
                return ERR_INVALID_MEM_ACCESS;
            }

            mem_read(vm, inst.loc, &vm->reg[inst.reg1], true);
        }
        break;

        case LDI:
        {
            if (inst.reg1 > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            if (inst.loc >= vm->mem_cap) {
                return ERR_INVALID_MEM_ACCESS;
            }

            mem_read(vm, inst.loc, &vm->reg[inst.reg1], false);
        }
        break;

        case ST:
        {
            if (inst.reg1 > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            if (inst.loc >= vm->mem_cap) {
                return ERR_INVALID_MEM_ACCESS;
            }

            mem_write(vm, inst.loc, vm->reg[inst.reg1], true);
        }
        break;

        case STI:
        {
            if (inst.reg1 > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }

            if (inst.loc >= vm->mem_cap) {
                return ERR_INVALID_MEM_ACCESS;
            }

            mem_write(vm, inst.loc, vm->reg[inst.reg1], false);
        }
        break;

        case CMP:
        {
            if (inst.reg1 > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }
            uint16_t reg1 = vm->reg[inst.reg1];
            
            if (inst.reg2 > REGISTER_LEN) {
                return ERR_INVALID_REGISTER;
            }
            uint16_t reg2 = vm->reg[inst.reg2];



            if (reg1 < reg2) {
                vm->reg[RBool] = 2;
            } else if (reg1 == reg2) {
                vm->reg[RBool] = 0;
            } else {
                vm->reg[RBool] = 1;
            }
        }
        break;

        case JMP:
        {
            if (inst.loc >= vm->insts_cap) {
                return ERR_INVALID_MEM_ACCESS;
            }

            vm->ip = inst.loc - 1; 
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

Error vm_load_program(VM *vm, Instruction *program, uint16_t icount) {
    if (icount >= vm->insts_cap) {
        return ERR_FAILED_LOAD_PROGRAM;
    }

    for (int i = 0; i < icount && i < vm->insts_cap; i++) {
        vm->insts[i] = program[i];
    }

    return ERR_OK;
}

Error vm_execute_program(VM *vm) {
    #ifdef DEBUG
    printf("===== PROGRAM INSTRUCTIONS =====\n");
    #endif

    Error err;
    while (vm->running) {
        err = vm_execute_instruction(vm, vm->insts[vm->ip]);

        if (err != ERR_OK) {
            return err;
        }

        vm->ip++;
    }
    return ERR_OK;
}
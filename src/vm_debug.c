#include <stdio.h>

#include "../include/vm_debug.h"

void dbg_print_memory(VM *vm) {
    printf("===== VM MEMORY =====\n");
    uint16_t i = 0;
    while (i < vm->inst_start) {
        if (vm->memory[i] != 0) {
            printf("%hu: %hu\n", i, vm->memory[i]);
        }
        i++;
    }
}

void dbg_print_instruction(uint16_t inst) {
    uint16_t op = inst >> 12;
    uint16_t reg;
    uint16_t loc;

    switch (op) {
        case MV:
            reg = (inst >> 9) & 0x7;
            loc = inst & 0x1FF;
            printf("OP: MV\nREG: %hu\nVAL: %hu\n", reg, loc);
            break;
        case LD:
            reg = (inst >> 9) & 0x7;
            loc = inst & 0x1FF;
            printf("OP: LD\nREG: %hu\nLOC: %hu\n", reg, loc);
            break;
        case ST:
            reg = (inst >> 9) & 0x7;
            loc = inst & 0x1FF;
            printf("OP: ST\nREG: %hu\nLOC: %hu\n", reg, loc);
            break;
        default:
            printf("UNKNOWN OPERATION: %hu\n", op);
            break;
    }

}
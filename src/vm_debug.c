#include <stdio.h>

#include "../include/vm_debug.h"

void dbg_print_memory(VM *vm) {
    printf("===== VM MEMORY =====\n");
    uint16_t i = 0;
    while (vm->memory[i] != 0) {
        printf("%hu: %hu\n", i, vm->memory[i]);
        i++;
    }
}
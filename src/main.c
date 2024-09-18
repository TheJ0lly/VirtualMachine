#include <stdio.h>

#include "../include/vm.h"

int main() {
    VM vm = {0};

    Error err = init_vm(&vm, DEFAULT_MEM_SPACE);

    if (err != ERR_OK) {
        printf("ERROR: %s\nExit code: -%d\n", error_as_string(err), err);
        return err;
    }

    Instruction program[] = {
        OP_MV(R1, 5),
        OP_ST(R1, 2),
        OP_JMP(7),


        OP_MV(R3, 69),
        OP_STI(R3, 2),
        
        OP_LDI(R2, 2),
        
        OP_CMP(R2, R3),
        OP_HALT,
    };

    err = vm_load_program(&vm, program, 8);
    if (err != ERR_OK) {
        printf("ERROR: %s\nExit code: -%d\n", error_as_string(err), err);
        return err;
    }

    err = vm_execute_program(&vm);

    if (err != ERR_OK) {
        printf("ERROR: %s\nExit code: -%d\n", error_as_string(err), err);
        return err;
    }

    dbg_print_memory(&vm);
    dbg_print_registers(&vm);
}
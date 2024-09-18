#include <stdio.h>

#include "../include/vm.h"
#include "../include/vm_debug.h"

int main() {
    VM vm = {0};

    Error err = init_vm(&vm, DEFAULT_MEM_SPACE, DEFAULT_INST_STARTPOINT, DEFAULT_DATA_STARTPOINT);

    if (err != ERR_OK) {
        printf("ERROR: %s\nExit code: -%d\n", error_as_string(err), err);
        return err;
    }

    err = vm_execute_instruction(&vm, 0x419);
    if (err != ERR_OK) {
        printf("ERROR: %s\nExit code: -%d\n", error_as_string(err), err);
        return err;
    }

    err = vm_execute_instruction(&vm, 0x2402);
    if (err != ERR_OK) {
        printf("ERROR: %s\nExit code: -%d\n", error_as_string(err), err);
        return err;
    }


    dbg_print_memory(&vm);    
}
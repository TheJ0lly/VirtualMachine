#include <stdio.h>

#include "../include/vm.h"
#include "../include/vm_debug.h"

int main() {
    VM vm = {0};

    Error err = init_vm(&vm, DEFAULT_MEM_SPACE, DEFAULT_START_POINT);

    if (err != ERR_OK) {
        printf("ERROR: %s\nExit code: -%d\n", error_as_string(err), err);
        return err;
    }

    mem_write(&vm, 0, 'H');
    mem_write(&vm, 1, 'e');
    mem_write(&vm, 2, 'l');
    mem_write(&vm, 3, 'l');
    mem_write(&vm, 4, 'o');
    mem_write(&vm, 5, 0);


    uint16_t addr = 0;
    uint16_t c = 0;
    bool run = true;
    while (run) {
        mem_read(&vm, addr, &c);

        if (c == 0) {
            break;
        }

        printf("%c", c);

        addr++;
    }
    printf("\n");

    dbg_print_memory(&vm);
    
}
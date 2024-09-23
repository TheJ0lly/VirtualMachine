#include <stdio.h>

#include "../include/vm.h"

void test_read_write(struct machine *vm) {
    write(vm, 0, 1);
    write(vm, 1, 1);

    printf("read2 value at 0: %d\nexpected value: 257\n\n", read2(vm, 0));

    write2(vm, 2, 257);

    printf("read value at 2: %d\nexpected value: 1\n\n", read(vm, 2));
    printf("read value at 3: %d\nexpected value: 1\n\n", read(vm, 3));

    printf("read2 value at 2: %d\nexpected value: 257\n\n", read2(vm, 2));
}

void test_push_pop(struct machine *vm) {
    push(vm, 10);
    printf("sp: %d\noffset: %d\n\n", vm->sp, vm->sp - default_sp_start);

    push(vm, 20);
    printf("sp: %d\noffset: %d\n\n", vm->sp, vm->sp - default_sp_start);    

    printf("sp: %d\noffset: %d\npopped value: %d\n\n", vm->sp, vm->sp - default_sp_start, pop(vm));

    printf("sp: %d\noffset: %d\npopped value: %d\n\n", vm->sp, vm->sp - default_sp_start, pop(vm));
    printf("sp: %d\noffset: %d\npopped value: %d\n\n", vm->sp, vm->sp - default_sp_start, pop(vm));
}


int main() {
    struct machine vm = {0};
    new_machine(&vm, max_memory_allowed);

    // test_read_write(&vm);

    test_push_pop(&vm);
}
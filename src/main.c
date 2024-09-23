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

    load_instruction(&vm, 0x80101);
    vm.ip = default_ip_start;

    execute_instruction(&vm, fetch_next_instruction(&vm));
    

    printf("ip: %d\n", vm.ip);
    vm.ip = default_ip_start;
    printf("sp: %d\n", vm.sp);
    printf("value on the stack: %d\nfirst byte: %d\nsecond byte: %d\n\n", read2(&vm, vm.sp-1), read(&vm, vm.sp-1), read(&vm, vm.sp));
    printf("first byte: %d\nsecond byte: %d\nlast 2 bytes value: %d\n", read(&vm, vm.ip), read(&vm, vm.ip+1), read2(&vm, vm.ip+2));

}
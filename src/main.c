#include <stdio.h>

#include "../include/vm.h"

void test1(struct machine *vm) {
    write(&vm, 0, 1);
    write(&vm, 1, 1);

    printf("read2 value at 0: %d\nexpected value: 257\n\n", read2(&vm, 0));

    write2(&vm, 2, 257);

    printf("read value at 2: %d\nexpected value: 1\n\n", read(&vm, 2));
    printf("read value at 3: %d\nexpected value: 1\n\n", read(&vm, 3));

    printf("read2 value at 2: %d\nexpected value: 257\n\n", read2(&vm, 2));
}


int main() {
    struct machine vm = {0};
    new_machine(&vm, max_memory_allowed);

    test1(&vm);
}
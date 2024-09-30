#include <stdio.h>
#include <malloc.h>
#include "../include/vm.h"

void test_read_write() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n===== TEST READ WRITE =====\n");

    write(&vm, 0, 1);
    write(&vm, 1, 1);

    printf("read2 value at 0: %d\nexpected value: 257\n\n", read2(&vm, 0));

    write2(&vm, 2, 257);

    printf("read value at 2: %d\nexpected value: 1\n\n", read(&vm, 2));
    printf("read value at 3: %d\nexpected value: 1\n\n", read(&vm, 3));

    printf("read2 value at 2: %d\nexpected value: 257\n", read2(&vm, 2));

    free(vm.memory);
}

void test_push() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST PUSH =====\n");

    execute_instruction(&vm, 0x80101);
    // we reset the ip.
    vm.ip = default_ip_start;

    printf("Stack value(16-bit): %d\nStack pointer: %d\n", read2(&vm, vm.sp-1), vm.sp);
    
    // we reset the sp.
    vm.sp = default_sp_start;

    vm.reg[R6] = 10;

    execute_instruction(&vm, 0xE00000);

    printf("Stack value(8-bit): %d\nStack pointer: %d\n", read(&vm, vm.sp), vm.sp);

    free(vm.memory);
}

void test_pop() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST POP =====\n");

    // We push 257 on the stack.
    execute_instruction(&vm, 0x80101);
    printf("Stack value(16-bit): %d\nStack pointer: %d\n\n", read2(&vm, vm.sp-1), vm.sp);

    // We pop the value.
    execute_instruction(&vm, 0x1000000);
    printf("Popped value(8-bit): %d\nStack pointer: %d\n", read(&vm, vm.sp + 1), vm.sp);
    // We reset the sp.
    vm.sp += 1;

    execute_instruction(&vm, 0x1080000);
    printf("Popped value(16-bit): %d\nStack pointer: %d\n", read2(&vm, vm.sp+1), vm.sp);

    // we increment the sp, to simulate the popping of 2 bytes.
    vm.sp += 2;

    execute_instruction(&vm, 0x1E80000);

    printf("Popped value from R6(16-bit): %d\nStack pointer: %d\n", vm.reg[R6], vm.sp);

    free(vm.memory);
}

void test_mov() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST MOV =====\n");

    execute_instruction(&vm, 0x20E0101);

    printf("R7: %d\n", vm.reg[R7]);

    execute_instruction(&vm, 0x2FC0101);

    printf("R7: %d -> R6: %d\n", vm.reg[R7], vm.reg[R6]);

    execute_instruction(&vm, 0x20C0018);

    printf("R6: %d\n", vm.reg[R6]);
}

void test_inc() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST INC =====\n");

    vm.reg[R7] = 9;

    printf("Initial R7: %d\n", vm.reg[R7]);
    execute_instruction(&vm, 0x3E00000);

    printf("Incremented R7: %d\n", vm.reg[R7]);
}

void test_dec() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST DEC =====\n");

    vm.reg[R7] = 9;

    printf("Initial R7: %d\n", vm.reg[R7]);
    execute_instruction(&vm, 0x4E00000);

    printf("Decremented R7: %d\n", vm.reg[R7]);
}

void test_neg() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST NEG =====\n");

    vm.reg[R7] = 28;

    printf("Initial R7: %d\n", vm.reg[R7]);
    execute_instruction(&vm, 0x5E00000);
    printf("Negated R7: %d\n", vm.reg[R7]);
    execute_instruction(&vm, 0x5E00000);
    printf("RENegated R7: %d\n", vm.reg[R7]);
}

void test_add() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST ADD =====\n");
    vm.reg[R7] = 0;

    execute_instruction(&vm, 0x6E0000A);
    printf("Initial R7: 0\nAdded R7: %d\n", vm.reg[R7]);

    vm.reg[R6] = 10;
    execute_instruction(&vm, 0x6CF0000);
    printf("Initial R6: 10\nAdded R7 into R6: %d\n", vm.reg[R6]);    
}

void test_sub() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST SUB =====\n");
    vm.reg[R7] = 20;
    execute_instruction(&vm, 0x7E0000A);
    printf("Initial R7: 20\nSub 10 from R7: %d\n", vm.reg[R7]);

    vm.reg[R6] = 79;
    execute_instruction(&vm, 0x7CF0000);
    printf("Initial R6: 79\nSub 10 from R6: %d\n", vm.reg[R6]);

}

void test_mul() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST MUL =====\n");
    vm.reg[R7] = 2;
    execute_instruction(&vm, 0x8E0000A);
    printf("Initial R7: 2\nMul 10 with R7: %d\n", vm.reg[R7]);

    vm.reg[R6] = 5;
    execute_instruction(&vm, 0x8CF0000);
    printf("Initial R6: 5\nMul R7 with R6: %d\n", vm.reg[R6]);
}

void test_div() {
    struct machine vm = {0};
    new_machine(&vm);

    printf("\n\n===== TEST DIV =====\n");
    vm.reg[R7] = 10;
    execute_instruction(&vm, 0x9E00002);
    printf("Initial R7: 10\nDivided R7 with 2: %d\n", vm.reg[R7]);

    vm.reg[R6] = 50;
    execute_instruction(&vm, 0x9CF0000);
    printf("Initial R6: 50\nDivided R6 with R7: %d\n", vm.reg[R6]);
}

void test_int() {
    printf("\n\n ===== INT DOES NOTHING YET =====\n");
}

void test_cmp() {
    struct machine vm = {0};
    new_machine(&vm);

    vm.reg[R7] = 2;
    vm.reg[R6] = 1;
    printf("\n\n===== TEST CMP =====\n");
    execute_instruction(&vm, 0xBF80000);

    printf("Compare reg: %d --- Expected 1\n", vm.reg[RCOMP]);

    vm.reg[R7] = 1;
    vm.reg[R6] = 2;

    execute_instruction(&vm, 0xBF80000);
    printf("Compare reg: %d --- Expected 2\n", vm.reg[RCOMP]);

    vm.reg[R7] = 1;
    vm.reg[R6] = 1;

    execute_instruction(&vm, 0xBF80000);
    printf("Compare reg: %d --- Expected 0\n", vm.reg[RCOMP]);
}

void test_jmps() {
    struct machine vm = {0};
    new_machine(&vm);
    printf("\n\n===== TEST JMP =====\n");

    execute_instruction(&vm, 0xC000001);
    printf("JMP IP: %d\n", vm.ip + 1);

    vm.reg[R7] = 2;
    vm.reg[R6] = 1;
    execute_instruction(&vm, 0xBF80000);
    execute_instruction(&vm, 0xD00000A);

    printf("Compare reg: %d --- JGR IP: %d\n", vm.reg[RCOMP], vm.ip + 1);

    vm.reg[R7] = 2;
    vm.reg[R6] = 2;

    execute_instruction(&vm, 0xBF80000);
    execute_instruction(&vm, 0xE000005);
    printf("Compare reg: %d --- JGE IP: %d\n", vm.reg[RCOMP], vm.ip + 1);

    vm.reg[R7] = 1;
    vm.reg[R6] = 2;

    execute_instruction(&vm, 0xBF80000);
    execute_instruction(&vm, 0xF000009);
    printf("Compare reg: %d --- JLS IP: %d\n", vm.reg[RCOMP], vm.ip + 1);


    vm.reg[R7] = 3;
    vm.reg[R6] = 3;

    execute_instruction(&vm, 0xBF80000);
    execute_instruction(&vm, 0x1000000E);
    printf("Compare reg: %d --- JLE IP: %d\n", vm.reg[RCOMP], vm.ip + 1);

}

int main() {
    printf("\n===== TESTING =====\n");

    test_read_write();
    test_push();
    test_pop();
    test_mov();
    test_inc();
    test_dec();
    test_neg();
    test_add();
    test_sub();
    test_mul();
    test_div();
    test_int();
    test_cmp();
    test_jmps();


    printf("\n===================\n\n");
}
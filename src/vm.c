#include "../include/vm.h"
#include <malloc.h>
#include <unistd.h>

/*
    Constants
*/
const uint16_t default_ip_start = 0xC350;       //50000
const uint16_t default_sp_start = 0x9C40;       //40000

void new_machine(struct machine *vm) {
    vm->memory = calloc(UINT16_MAX, sizeof(uint8_t));
    vm->ip = default_ip_start;
    vm->sp = default_sp_start;
    vm->running = true;

    /*
    We set it to 0, because errors will set this flag to a non-zero value.
    */
    vm->reg[RERROR] = 0;
}

uint8_t read1(struct machine *vm, uint16_t addr) {
    return vm->memory[addr];
}

uint16_t read2(struct machine *vm, uint16_t addr) {
    return ((uint16_t)vm->memory[addr]) | (((uint16_t)vm->memory[addr+1]) << 8);
}

void write1(struct machine *vm, uint16_t addr, uint8_t val) {
    vm->memory[addr] = val;
}

void write2(struct machine *vm, uint16_t addr, uint16_t val) {
    vm->memory[addr] = (uint8_t)(val & 0x00FF);
    vm->memory[addr + 1] = (uint8_t)(val >> 8);
}

char *err_as_string(enum Error err) {
    switch (err) {
    case OK:
        return "OK - no error ocurred";
    case EMPTY_STACK:
        return "EMPTY STACK - performed stack operation with no or not enough elements";
    case STACK_OVERFLOW:
        return "STACK OVERFLOW - stack pointer has surpassed its allowed space";
    case UNKNOWN_OP:
        return "UNKNOWN OPERATION PERFORMED";
    default:
        return "UNKNOWN STATE OF VM";
    }
}

void load_instruction(struct machine *vm, uint32_t instruction) {
    enum Operation op = GET_BYTE_1(instruction);

    switch (op) {
    // 4 bytes instructions    
    case PUSH:
    case MOV:
    case ADD:
    case SUB:
    case MUL:
    case DIV:
    case INT:
        vm->memory[vm->ip++] = op;
        vm->memory[vm->ip++] = GET_BYTE_2(instruction);
        vm->memory[vm->ip++] = GET_BYTE_3(instruction);
        vm->memory[vm->ip++] = GET_BYTE_4(instruction);
        break;
    
    // Jumps
    case JMP:
    case JGR:
    case JGE:
    case JLS:
    case JLE:
        vm->memory[vm->ip++] = op;
        vm->memory[vm->ip++] = GET_BYTE_3(instruction);
        vm->memory[vm->ip++] = GET_BYTE_4(instruction);
        break;

    // 2 bytes instructions
    case POP:
    case INC:
    case DEC:
    case NEG:
    case CMP:
        vm->memory[vm->ip++] = op;
        vm->memory[vm->ip++] = GET_BYTE_2(instruction);
        break;

    // 1 byte instructions
    case HALT:
        vm->memory[vm->ip++] = op;
        break;

    default:
        printf("unknown op: %d\n", op);
        break; 
    }    
}

uint32_t fetch_next_instruction(struct machine *vm) {
    enum Operation op = read1(vm, vm->ip++);
    uint32_t instruction = op;

    switch (op) {
    // 4 bytes instructions    
    case PUSH:
    case MOV:
    case ADD:
    case SUB:
    case MUL:
    case DIV:
    case INT:
        instruction = (instruction << 8) | read1(vm, vm->ip++);
        instruction = (instruction << 8) | read1(vm, vm->ip++);
        instruction = (instruction << 8) | read1(vm, vm->ip++);
        break;
    
    // Jumps
    case JMP:
    case JGR:
    case JGE:
    case JLS:
    case JLE:
        instruction = instruction << 8; // Null space
        instruction = (instruction << 8) | read1(vm, vm->ip++);
        instruction = (instruction << 8) | read1(vm, vm->ip++);
        break;

    // 2 bytes instructions
    case POP:
    case INC:
    case DEC:
    case NEG:
    case CMP:
        instruction = (instruction << 8) | read1(vm, vm->ip++);
        instruction = instruction << 16;
        break;

    // 1 byte instructions
    case HALT: // Nothing else to do.
        break;

    default:
        printf("unknown op: %d\n", op);
        break; 
    }

    return instruction;
}

void load_program(struct machine *vm, uint32_t *program, uint16_t size) {
    for (int i = 0; i < size; i++) {
        load_instruction(vm, program[i]);
    }
}

void reset_ip(struct machine *vm) {
    vm->ip = default_ip_start;
}

void handle_interrupt(struct machine *vm, uint8_t type, uint8_t size) {
    switch (type)
    {
    case 0: // Input
        switch (size)
        {
        case 1: // 1 byte
            fread(&vm->reg[R0], 1, 1, stdin);
        break;

        case 2: // 2 bytes
            fread(&vm->reg[R0], 1, 2, stdin);
        break;

        case 3: // string
        {
            // Get the number of elements to read. 
            uint16_t count = vm->reg[R1];
            uint16_t index = 0;
            char c = 0;
            while (index < count - 1) {
                if (fread(&c, 1, 1, stdin) > 0) {
                    write1(vm, vm->reg[R0] + index, c);
                } else {
                    vm->reg[RERROR] = INPUT_ERROR;
                    return;
                }
                index++;
            }

            // Add null-char for safety. Might delete it later?
            write1(vm, vm->reg[R0] + index, 0);
        }
        break;

        default:
        vm->reg[RERROR] = UNKNOWN_DATA_TYPE;
        return;
        }

    break;
    
    case 1: // Output
        switch (size)
        {
        case 1: // 1 byte
        {
            uint8_t c = read1(vm, vm->reg[R0]); 
            fwrite(&c, 1, 1, stdin);
        }
        break;

        case 2: // 2 bytes
        {
            uint16_t c = read2(vm, vm->reg[R0]);
            fwrite(&c, 2, 1, stdin);
        }
        break;

        case 3: // string
        {
            // Get the number of elements to write. 
            uint16_t count = vm->reg[R1];
            uint16_t index = 0;
            while (index < count - 1) {
                if (printf("%c", read1(vm, vm->reg[R0] + index)) == 0) {
                    vm->reg[RERROR] = OUTPUT_ERROR;
                    return;
                }
                index++;
            }
        }
        break;

        default:
        vm->reg[RERROR] = UNKNOWN_DATA_TYPE;
        return;
        }
    break;
    
    default:
        break;
    }
}

void execute_instruction(struct machine *vm, uint32_t instruction) {
    enum Operation op = GET_BYTE_1(instruction);

    switch (op) {
    case PUSH:
    { 
        if (vm->sp + 1 == default_ip_start) {
            vm->reg[RERROR] = STACK_OVERFLOW;
            fprintf(stderr, "cannot push - stack overflow (addr: %d)\n", vm->sp + 1);
            return;
        }

        bool isreg = (instruction & 0x00800000) >> 23;
        bool is16 = (instruction & 0x00080000) >> 19;
        uint16_t value = (uint16_t)(instruction & 0x0000FFFF);

        if (isreg) {
            uint8_t reg = (instruction & 0x00700000) >> 20;
            value = vm->reg[reg];
        }



        // Create the offset
        vm->sp++;
        if (is16) {
            write2(vm, vm->sp, value);
            // We write 2 bytes, so we have a 2 bytes offset.
            vm->sp += 1;
        } else {
            write1(vm, vm->sp, (uint8_t)(value & 0x00FF));
        }
    }
    break;

    case POP:
    {
        if (vm->sp == default_sp_start) {
            vm->reg[RERROR] = EMPTY_STACK;
            fprintf(stderr, "cannot pop - stack is empty (addr: %d)\n", vm->sp - 1);
            return;
        }

        bool isreg = (instruction & 0x00800000) >> 23;
        bool is16 = (instruction & 0x00080000) >> 19;
        uint8_t reg = (instruction & 0x00700000) >> 20;

        if (is16) {
            // We get the last 2 bytes of the stack.
            uint16_t stack_value = read2(vm, vm->sp - 1);

            if (isreg)
                vm->reg[reg] = stack_value;

            // We decrement by 2, because we pop 2 bytes.
            vm->sp -= 2;
        } else {
            // We get the last byte of the stack.
            uint8_t stack_value = read1(vm, vm->sp);

            if (isreg)
                vm->reg[reg] = stack_value;

            // We decrement by 1.
            vm->sp -= 1;
        }
    }
    break;

    case MOV:
    {
        // Signal if the value will come from a register.
        bool isreg = (instruction & 0x00800000) >> 23;

        // The register from which we get the value.
        uint8_t from_reg = (instruction & 0x00700000) >> 20;
        
        // The register in which we will add.
        uint8_t to_reg = (instruction & 0x000E0000) >> 17;

        // The last 16 bits.
        uint16_t value = (instruction & 0x0000FFFF);

        if (isreg)
            vm->reg[to_reg] = vm->reg[from_reg];
        else
            vm->reg[to_reg] = value;
    }
    break;

    case INC:
    {
        uint8_t reg = (instruction & 0x00E00000) >> 21;

        vm->reg[reg] += 1;
    }
    break;

    case DEC:
    {
        uint8_t reg = (instruction & 0x00E00000) >> 21;

        vm->reg[reg] -= 1;
    }
    break;

    case NEG:
    {
        uint8_t reg = (instruction & 0x00E00000) >> 21;

        vm->reg[reg] = (vm->reg[reg] ^ 0xFFFF) + 1;
    }
    break;

    case ADD:
    {
        // The register in which we add.
        uint8_t reg = (instruction & 0x00E00000) >> 21;

        // Signal if the value will come from a register.
        uint8_t is_from_reg = (instruction & 0x00080000) >> 19;
        uint8_t from_reg = (instruction & 0x00070000) >> 16;

        // Last 16 bits.
        uint16_t value = (instruction & 0x0000FFFF);

        if (is_from_reg)
            vm->reg[reg] += vm->reg[from_reg];
        else
            vm->reg[reg] += value;

    }
    break;

    case SUB:
    {
        // The register in which we add.
        uint8_t reg = (instruction & 0x00E00000) >> 21;

        // Signal if the value will come from a register.
        uint8_t is_from_reg = (instruction & 0x00080000) >> 19;
        uint8_t from_reg = (instruction & 0x00070000) >> 16;

        // Last 16 bits.
        uint16_t value = (instruction & 0x0000FFFF);

        if (is_from_reg)
            vm->reg[reg] -= vm->reg[from_reg];
        else
            vm->reg[reg] -= value;

    }
    break;

    case MUL:
    {
        // The register in which we add.
        uint8_t reg = (instruction & 0x00E00000) >> 21;

        // Signal if the value will come from a register.
        uint8_t is_from_reg = (instruction & 0x00080000) >> 19;
        uint8_t from_reg = (instruction & 0x00070000) >> 16;

        // Last 16 bits.
        uint16_t value = (instruction & 0x0000FFFF);

        if (is_from_reg)
            vm->reg[reg] *= vm->reg[from_reg];
        else
            vm->reg[reg] *= value;

    }
    break;

    case DIV:
    {
        // The register in which we add.
        uint8_t reg = (instruction & 0x00E00000) >> 21;

        // Signal if the value will come from a register.
        uint8_t is_from_reg = (instruction & 0x00080000) >> 19;
        uint8_t from_reg = (instruction & 0x00070000) >> 16;

        // Last 16 bits.
        uint16_t value = (instruction & 0x0000FFFF);

        if (is_from_reg)
            vm->reg[reg] /= vm->reg[from_reg];
        else
            vm->reg[reg] /= value;

    }
    break;

    case INT:
    {
        uint8_t type = (instruction & 0x00F00000) >> 20;
        uint8_t size = (instruction & 0x000F0000) >> 16;

        handle_interrupt(vm, type, size);

    }
    break;

    case CMP:
    {
        uint8_t reg1 = (instruction & 0x00E00000) >> 21;
        uint8_t reg2 = (instruction & 0x001E0000) >> 18;

        if (vm->reg[reg1] > vm->reg[reg2])
            vm->reg[RCOMP] = 1;
        else if (vm->reg[reg1] < vm->reg[reg2])
            vm->reg[RCOMP] = 2;
        else
            vm->reg[RCOMP] = 0;
    }
    break;

    case JMP:
    {
        // We decrement by 1, because we will increment in the execution loop.
        uint16_t location = (instruction & 0x0000FFFF);

        vm->ip = location;
    }
    break;

    case JGR:
    {
        // We decrement by 1, because we will increment in the execution loop.
        uint16_t location = (instruction & 0x0000FFFF);

        /*
            We check for 1, because we compare REG1 and REG2, 
            and greater means that REG1 is bigger.
        */ 
        if (vm->reg[RCOMP] == 1)
            vm->ip = location;
    }
    break;

    case JGE:
    {
        // We decrement by 1, because we will increment in the execution loop.
        uint16_t location = (instruction & 0x0000FFFF);

        /*
            We check for 1 and for 0, because we compare REG1 and REG2, 
            and greater means that REG1 is bigger.
        */ 
        if (vm->reg[RCOMP] == 1 || vm->reg[RCOMP] == 0)
            vm->ip = location;
    }
    break;

    case JLS:
    {
        // We decrement by 1, because we will increment in the execution loop.
        uint16_t location = (instruction & 0x0000FFFF);

        /*
            We check for 2, because we compare REG1 and REG2, 
            and less means that REG1 is smaller.
        */ 
        if (vm->reg[RCOMP] == 2)
            vm->ip = location;
    }
    break;

    case JLE:
    {
        // We decrement by 1, because we will increment in the execution loop.
        uint16_t location = (instruction & 0x0000FFFF);

        /*
            We check for 1 and for 0, because we compare REG1 and REG2, 
            and less means that REG1 is smaller.
        */ 
        if (vm->reg[RCOMP] == 2 || vm->reg[RCOMP] == 0)
            vm->ip = location;
    }
    break;

    case HALT:
    vm->running = false;
    break;

    default:
    printf("unknown op: %d\n", op);
    vm->reg[RERROR] = UNKNOWN_OP;
    break;
    }
}

void execute_program(struct machine *vm) {
    while (vm->running) {
        execute_instruction(vm, fetch_next_instruction(vm));

        if (vm->reg[RERROR] != 0) {
            printf("ERROR: %s\nexit code %d\n", err_as_string(vm->reg[RERROR]), vm->reg[RERROR]);
            break;
        }
    }
}

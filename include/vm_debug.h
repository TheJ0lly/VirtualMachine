#ifndef _VM_DEBUG_H_
#define _VM_DEBUG_H_

#include "vm.h"

void dbg_print_all_instructions(VM *vm);
void dbg_print_memory(VM *vm);

void dbg_print_instruction(uint16_t inst);

#endif
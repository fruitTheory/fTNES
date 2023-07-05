#pragma once

#include "config.h"

struct MOS_6502;

// 256 byte stack located between $0100 and $01FF
struct MOS_6502_stack
{
    unsigned short stack[stack_max_size];
};

// stackpointer is an 8 bit register and holds low 8 bits of next free location on stack
void MOS_6502_stack_push(struct MOS_6502* MOS_6502, unsigned short val);
void MOS_6502_stack_pop(struct MOS_6502* MOS_6502, unsigned short val);
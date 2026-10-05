#pragma once

#include <common.h>
#include "instructions.h"

#define CPU_FLAG_Z BIT(ctx->regs.f, 7)
#define CPU_FLAG_C BIT(ctx->regs.f, 4)

typedef struct {
    u8 a;
    u8 f;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 h;
    u8 l;
    u16 pc;
    u16 sp;
} cpu_reg;

typedef struct {
    cpu_reg regs;

    u16 fetched_data;
    u8 cur_opcode;
    instruction *cur_instr;

    u16 mem_dest;
    bool mem_write;   // True if destination is memory, false if using registers

    bool halted;      // Paused
    bool stepping;    // For debugging
    bool int_master_enabled;  // Interrupt master enabled (IME)
} cpu_context;

typedef void (*IN_PROC)(cpu_context *);
IN_PROC instr_get_processor(instr_type type);

void cpu_init();
bool cpu_step();

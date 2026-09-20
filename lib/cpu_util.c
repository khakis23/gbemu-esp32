#include "cpu_util.h"


extern cpu_context cpu_ctx;

u16 cpu_read_reg(const reg_type rt) {
    switch(rt) {
        case RT_A: return cpu_ctx.regs.a;
        case RT_F: return cpu_ctx.regs.f;
        case RT_B: return cpu_ctx.regs.b;
        case RT_C: return cpu_ctx.regs.c;
        case RT_D: return cpu_ctx.regs.d;
        case RT_E: return cpu_ctx.regs.e;
        case RT_H: return cpu_ctx.regs.h;
        case RT_L: return cpu_ctx.regs.l;

        // 16-bit Virtual Register Pairs (High << 8 | Low)
        case RT_AF: return (cpu_ctx.regs.a << 8) | cpu_ctx.regs.f;
        case RT_BC: return (cpu_ctx.regs.b << 8) | cpu_ctx.regs.c;
        case RT_DE: return (cpu_ctx.regs.d << 8) | cpu_ctx.regs.e;
        case RT_HL: return (cpu_ctx.regs.h << 8) | cpu_ctx.regs.l;

        // 16-bit Pointers
        case RT_PC: return cpu_ctx.regs.pc;
        case RT_SP: return cpu_ctx.regs.sp;

        default: return 0;
    }
}
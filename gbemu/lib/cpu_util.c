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

        default:
            fprintf(stderr, "Invalid register type: %d\n", rt);
            return 0;
    }
}

void cpu_set_reg(reg_type rt, u16 value) {
    switch(rt) {
        // 8-bit Registers
        case RT_A: cpu_ctx.regs.a = value & 0xFF; break;
        case RT_F: cpu_ctx.regs.f = value & 0xF0; break;  // Bottom nibble of F is ALWAYS 0!
        case RT_B: cpu_ctx.regs.b = value & 0xFF; break;
        case RT_C: cpu_ctx.regs.c = value & 0xFF; break;
        case RT_D: cpu_ctx.regs.d = value & 0xFF; break;
        case RT_E: cpu_ctx.regs.e = value & 0xFF; break;
        case RT_H: cpu_ctx.regs.h = value & 0xFF; break;
        case RT_L: cpu_ctx.regs.l = value & 0xFF; break;

        // 16-bit Register Pairs
        case RT_AF:
            cpu_ctx.regs.a = value >> 8;
            cpu_ctx.regs.f = value & 0xF0; // Bottom of F is 0
            break;
        case RT_BC:
            cpu_ctx.regs.b = value >> 8;
            cpu_ctx.regs.c = value & 0xFF;
            break;
        case RT_DE:
            cpu_ctx.regs.d = value >> 8;
            cpu_ctx.regs.e = value & 0xFF;
            break;
        case RT_HL:
            cpu_ctx.regs.h = value >> 8;
            cpu_ctx.regs.l = value & 0xFF;
            break;

        // 16-bit Pointers
        case RT_PC: cpu_ctx.regs.pc = value; break;
        case RT_SP: cpu_ctx.regs.sp = value; break;
        default: fprintf(stderr, "Invalid register type: %d\n", rt); break;
    }
}

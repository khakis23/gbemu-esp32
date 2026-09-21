#include <cpu.h>
#include <emu.h>
#include <bus.h>
#include <common.h>

#include "cpu_util.h"


/* Flag Management Helper */
void cpu_set_flags(cpu_context *ctx, char z, char n, char h, char c) {
    if (z != -1) {
        BIT_SET(ctx->regs.f, 7, z);
    }
    if (n != -1) {
        BIT_SET(ctx->regs.f, 6, n);
    }
    if (h != -1) {
        BIT_SET(ctx->regs.f, 5, h);
    }
    if (c != -1) {
        BIT_SET(ctx->regs.f, 4, c);
    }
}
/* Condition Code Evaluator */
static bool check_cond(cpu_context *ctx) {
    const bool z = CPU_FLAG_Z;
    const bool c = CPU_FLAG_C;
    switch(ctx->cur_instr->cond) {
        case CT_NONE: return true;
        case CT_C:    return c;
        case CT_NC:   return !c;
        case CT_Z:    return z;
        case CT_NZ:   return !z;
    }
    return false;
}

/* Default / Invalid Instruction Handler */
static void proc_none(cpu_context *ctx) {
    fprintf(stderr, "INVALID INSTRUCTION! Opcode: 0x%02X at PC: 0x%04X\n", ctx->cur_opcode, ctx->regs.pc);
    exit(-7);
}
static void proc_nop(cpu_context *ctx) {
    /* No OP */
}
static void proc_di(cpu_context *ctx) {
    ctx->int_master_enabled = false;
}
static void proc_xor(cpu_context *ctx) {
    ctx->regs.a ^= (ctx->fetched_data & 0xFF);
    cpu_set_flags(ctx, ctx->regs.a == 0, 0, 0, 0);
}
static void proc_jp(cpu_context *ctx) {
    if (check_cond(ctx)) {
        ctx->regs.pc = ctx->fetched_data;
        emu_cycle(1);
    }
}

/* Handles all standard LDs */
static void proc_ld(cpu_context *ctx) {
    // 1. Reg to mem
    if (ctx->mem_write) {
        // Write addr is 16-bit
        if (ctx->cur_instr->reg_2 >= RT_AF) {
            bus_write16(ctx->mem_dest, ctx->fetched_data);
            emu_cycle(2);
        }
        // 8-bit
        else {
            bus_write(ctx->mem_dest, ctx->fetched_data);
            emu_cycle(1);
        }
    }

    // 2. Special Case: perform operation on the stack item
    else if (ctx->cur_instr->mode == AM_HL_SPR) {
        const u16 sp = cpu_read_reg(RT_SP);
        const u8 offset_raw = ctx->fetched_data & 0xFF;

        // Calculate carry flags
        bool h_carry = (sp & 0x0F) + (offset_raw & 0x0F) >= 0x10;
        bool carry = (sp & 0xFF) + (offset_raw & 0xFF) >= 0x100;
        cpu_set_flags(ctx, 0, 0, h_carry, carry);

        // Computer signed SP offset and save to HL reg
        //                      |—> (remember that SP can go up and down)
        const u16 res = sp + (int8_t)offset_raw;  // signed!
        cpu_set_reg(RT_HL, res);  // always save to HL
    }

    // 3. Reg to reg
    else {
        cpu_set_reg(ctx->cur_instr->reg_1, ctx->fetched_data);
    }
}

/* LDs for High Memory (only supports reg A) */
static void proc_ldh(cpu_context *ctx) {
    // GB High memory assumes top bits (FF)
    // Read into reg A
    if (ctx->cur_instr->reg_1 == RT_A) {
        cpu_set_reg(RT_A, bus_read(0xFF00 | ctx->fetched_data));
    }
    // Write
    else {
        bus_write(ctx->mem_dest, ctx->regs.a);
    }
    emu_cycle(1);
}

static void proc_inc(cpu_context *ctx) {}
static void proc_dec(cpu_context *ctx) {}
static void proc_add(cpu_context *ctx) {}
static void proc_adc(cpu_context *ctx) {}
static void proc_sub(cpu_context *ctx) {}
static void proc_sbc(cpu_context *ctx) {}
static void proc_and(cpu_context *ctx) {}
static void proc_or(cpu_context *ctx) {}
static void proc_cp(cpu_context *ctx) {}
static void proc_cb(cpu_context *ctx) {}
static void proc_rlca(cpu_context *ctx) {}
static void proc_rrca(cpu_context *ctx) {}
static void proc_rla(cpu_context *ctx) {}
static void proc_rra(cpu_context *ctx) {}
static void proc_stop(cpu_context *ctx) {}
static void proc_daa(cpu_context *ctx) {}
static void proc_cpl(cpu_context *ctx) {}
static void proc_scf(cpu_context *ctx) {}
static void proc_ccf(cpu_context *ctx) {}
static void proc_halt(cpu_context *ctx) {}
static void proc_push(cpu_context *ctx) {}
static void proc_pop(cpu_context *ctx) {}
static void proc_jr(cpu_context *ctx) {}
static void proc_call(cpu_context *ctx) {}
static void proc_ret(cpu_context *ctx) {}
static void proc_reti(cpu_context *ctx) {}
static void proc_rst(cpu_context *ctx) {}
static void proc_ei(cpu_context *ctx) {}
static void proc_jphl(cpu_context *ctx) {}

/* Processor Dispatch Table */
static IN_PROC processors[] = {
    [IN_NONE] = proc_none,
    [IN_NOP]  = proc_nop,
    [IN_LD]   = proc_ld,
    [IN_INC]  = proc_inc,
    [IN_DEC]  = proc_dec,
    [IN_RLCA] = proc_rlca,
    [IN_ADD]  = proc_add,
    [IN_RRCA] = proc_rrca,
    [IN_STOP] = proc_stop,
    [IN_RLA]  = proc_rla,
    [IN_JR]   = proc_jr,
    [IN_RRA]  = proc_rra,
    [IN_DAA]  = proc_daa,
    [IN_CPL]  = proc_cpl,
    [IN_SCF]  = proc_scf,
    [IN_CCF]  = proc_ccf,
    [IN_HALT] = proc_halt,
    [IN_ADC]  = proc_adc,
    [IN_SUB]  = proc_sub,
    [IN_SBC]  = proc_sbc,
    [IN_AND]  = proc_and,
    [IN_XOR]  = proc_xor,
    [IN_OR]   = proc_or,
    [IN_CP]   = proc_cp,
    [IN_POP]  = proc_pop,
    [IN_JP]   = proc_jp,
    [IN_PUSH] = proc_push,
    [IN_RET]  = proc_ret,
    [IN_CB]   = proc_cb,
    [IN_CALL] = proc_call,
    [IN_RETI] = proc_reti,
    [IN_LDH]  = proc_ldh,
    [IN_JPHL] = proc_jphl,
    [IN_DI]   = proc_di,
    [IN_EI]   = proc_ei,
    [IN_RST]  = proc_rst,
    [IN_ERR]  = proc_none
};

IN_PROC instr_get_processor(const instr_type type) {
    return processors[type];
}
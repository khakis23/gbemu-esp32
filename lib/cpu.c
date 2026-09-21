#include <cpu.h>
#include <common.h>
#include "bus.h"
#include "cpu_util.h"
#include "emu.h"

static void fetch_data();
static void execute();

cpu_context cpu_ctx = {0};

void cpu_init() {
    cpu_ctx.regs.a = 0x01;
    cpu_ctx.regs.pc = 0x0100;
    cpu_ctx.regs.sp = 0xFFFE;
}

/* Core CPU function */
bool cpu_step() {
    if (cpu_ctx.halted) {
        printf("CPU Halted!\n");
        return false;
    }

    // 1. fetch
    cpu_ctx.cur_opcode = bus_read(cpu_ctx.regs.pc++);
    // 2. decode
    cpu_ctx.cur_instr = instruction_by_opcode(cpu_ctx.cur_opcode);
    emu_cycle(1);  // NOTE: 0xCB OP codes require 2 cycles

    // Debug Logging
    #if VERBOSE
        const u16 pc = cpu_ctx.regs.pc - 1;
        printf("%04X: %-7s (%02X %02X %02X) A: %02X B: %02X C: %02X\n",
            pc,
            instr_name(cpu_ctx.cur_instr->type),
            cpu_ctx.cur_opcode,
            bus_read(pc + 1),
            bus_read(pc + 2),
            cpu_ctx.regs.a,
            cpu_ctx.regs.b,
            cpu_ctx.regs.c);
    #endif

    // 3. fetch data
    fetch_data();  // up to +2 EMU cycles
    // 4. execute
    execute();

    return true;
}

static void fetch_data() {
    cpu_ctx.mem_dest = 0x0;
    cpu_ctx.mem_write = false;

    if (cpu_ctx.cur_instr == NULL) {
        fprintf(stderr, "NULL instructions for opcode: %02X\n", cpu_ctx.cur_opcode);
        return;
    }

    // Match the current addr mode to the corresponding operation
    switch (cpu_ctx.cur_instr->mode) {
        case AM_IMP:    // Implied (e.g. NOP)
            return;

        case AM_R:     // Single Reg Operation
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_instr->reg_1);
            return;

        case AM_R_R:
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_instr->reg_2);
            return;

        case AM_R_D8:  // Read 8-bit immediate
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc++);
            emu_cycle(1);
            return;

        case AM_A8_R:
            cpu_ctx.mem_dest = bus_read(cpu_ctx.regs.pc++) | 0xFF00;
            cpu_ctx.mem_write = true;
            emu_cycle(1);
            return;

        case AM_D16: {  // Read 16-bit immediate
            cpu_ctx.fetched_data = bus_read16(cpu_ctx.regs.pc);
            emu_cycle(2);
            cpu_ctx.regs.pc += 2;
            return;
        }

        case AM_MR_R: {
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_instr->reg_2);
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_instr->reg_1);
            // special case: extend 8-bit C register to 16-bit
            if (cpu_ctx.cur_instr->reg_1 == RT_C) {
                cpu_ctx.mem_dest |= 0xFF00;
            }
            cpu_ctx.mem_write = true;
            return;
        }

        case AM_R_D16: {
            cpu_ctx.fetched_data = bus_read16(cpu_ctx.regs.pc);
            emu_cycle(2);
            cpu_ctx.regs.pc += 2;
            return;
        }

        case AM_D8:
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc++);
            emu_cycle(1);
            return;

        case AM_R_MR: {
            u16 addr = cpu_read_reg(cpu_ctx.cur_instr->reg_2);
            // special case: extend 8-bit C register to 16-bit
            if (cpu_ctx.cur_instr->reg_2 == RT_C) {
                addr |= 0xFF00;
            }
            cpu_ctx.fetched_data = bus_read(addr);
            emu_cycle(1);
            return;
        }

        case AM_R_HLI:
            cpu_ctx.fetched_data = bus_read(cpu_read_reg(cpu_ctx.cur_instr->reg_2));
            emu_cycle(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_R_HLD:
            cpu_ctx.fetched_data = bus_read(cpu_read_reg(cpu_ctx.cur_instr->reg_2));
            emu_cycle(1);
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        case AM_HLI_R:
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_instr->reg_2);
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_instr->reg_1);
            cpu_ctx.mem_write = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) + 1);
            return;

        case AM_HLD_R:
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_instr->reg_2);
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_instr->reg_1);
            cpu_ctx.mem_write = true;
            cpu_set_reg(RT_HL, cpu_read_reg(RT_HL) - 1);
            return;

        case AM_R_A8:
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc++);
            emu_cycle(1);
            return;

        case AM_HL_SPR:
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc++);
            emu_cycle(1);
            return;

        case AM_D16_R:
        case AM_A16_R: {
            cpu_ctx.mem_dest = bus_read16(cpu_ctx.regs.pc);
            cpu_ctx.mem_write = true;
            emu_cycle(2);
            cpu_ctx.regs.pc += 2;
            cpu_ctx.fetched_data = cpu_read_reg(cpu_ctx.cur_instr->reg_2);
            return;
        }

        case AM_MR_D8:
            cpu_ctx.fetched_data = bus_read(cpu_ctx.regs.pc++);
            emu_cycle(1);
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_instr->reg_1);
            cpu_ctx.mem_write = true;
            return;

        case AM_MR:
            cpu_ctx.mem_dest = cpu_read_reg(cpu_ctx.cur_instr->reg_1);
            cpu_ctx.mem_write = true;
            cpu_ctx.fetched_data = bus_read(cpu_read_reg(cpu_ctx.cur_instr->reg_1));
            emu_cycle(1);
            return;

        case AM_R_A16: {
            const u16 addr = bus_read16(cpu_ctx.regs.pc);
            emu_cycle(2);
            cpu_ctx.regs.pc += 2;
            cpu_ctx.fetched_data = bus_read(addr);
            emu_cycle(1);
            return;
        }

        default: {
            fprintf(stderr, "Unknown addressing mode: %d\n", cpu_ctx.cur_instr->mode);
            exit(-7);
        }
    }
}

static void execute() {
    const IN_PROC proc = instr_get_processor(cpu_ctx.cur_instr->type);

    if (proc == NULL) {
        fprintf(stderr, "No process for this instructions: %d\n", cpu_ctx.cur_instr->type);
        exit(-7);
    }

    proc(&cpu_ctx);
}

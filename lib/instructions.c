#include "instructions.h"


static instruction instructions[0x100] = {
    [0x00] = { IN_NOP, AM_IMP },
    [0x05] = { IN_DEC, AM_R, RT_B },
    [0x0E] = { IN_LD,  AM_R_D8, RT_C },
    [0xAF] = { IN_XOR, AM_R, RT_A },
    [0xC3] = { IN_JP,  AM_D16 },
    [0xF3] = { IN_DI,  AM_IMP }
};

static const char *instr_names[0x100] = {
    "<NONE>",
    "NOP",
    "LD",
    "INC",
    "DEC",
    "RLCA",
    "ADD",
    "RRCA",
    "STOP",
    "RLA",
    "JR",
    "RRA",
    "DAA",
    "CPL",
    "SCF",
    "CCF",
    "HALT",
    "ADC",
    "SUB",
    "SBC",
    "AND",
    "XOR",
    "OR",
    "CP",
    "POP",
    "JP",
    "PUSH",
    "RET",
    "CB",
    "CALL",
    "RETI",
    "LDH",
    "JPHL",
    "DI",
    "EI",
    "RST",
    "IN_ERR",
    "RLC",
    "RRC",
    "RL",
    "RR",
    "SLA",
    "SRA",
    "SWAP",
    "SRL",
    "BIT",
    "RES",
    "SET"
};

instruction *instruction_by_opcode(const u8 opcode) {
    return &instructions[opcode];
}

const char *instr_name(const instr_type t) {
    return instr_names[t];
}
#ifndef GBEMU_INSTRUCTIONS_H
#define GBEMU_INSTRUCTIONS_H

#include <common.h>


/* Addressing Modes: Define how operands are retrieved or stored */
typedef enum {
    AM_IMP,     /* Implied: operand is inherent to the instruction (e.g. NOP, DI, EI) */
    AM_R_D16,   /* Register <- 16-bit immediate (e.g. LD BC, d16) */
    AM_R_R,     /* Register <- Register (e.g. LD A, B) */
    AM_MR_R,    /* Memory at [Register] <- Register (e.g. LD (HL), A or LD (C), A) */
    AM_R,       /* Single Register operand (e.g. INC B, DEC A) */
    AM_R_D8,    /* Register <- 8-bit immediate (e.g. LD C, d8) */
    AM_R_MR,    /* Register <- Memory at [Register] (e.g. LD A, (HL)) */
    AM_R_HLI,   /* Register <- Memory at [HL], then HL is incremented (e.g. LD A, (HL+)) */
    AM_R_HLD,   /* Register <- Memory at [HL], then HL is decremented (e.g. LD A, (HL-)) */
    AM_HLI_R,   /* Memory at [HL] <- Register, then HL is incremented (e.g. LD (HL+), A) */
    AM_HLD_R,   /* Memory at [HL] <- Register, then HL is decremented (e.g. LD (HL-), A) */
    AM_R_A8,    /* Register <- Memory at high I/O address [0xFF00 + a8] (e.g. LDH A, (a8)) */
    AM_A8_R,    /* Memory at high I/O address [0xFF00 + a8] <- Register (e.g. LDH (a8), A) */
    AM_HL_SPR,  /* HL <- SP + signed 8-bit offset (e.g. LD HL, SP+r8) */
    AM_D16,     /* 16-bit immediate address or value (e.g. JP a16, CALL a16) */
    AM_D8,      /* 8-bit immediate value */
    AM_D16_R,   /* Memory at [16-bit immediate address] <- Register (e.g. LD (a16), SP or LD (a16), A) */
    AM_MR_D8,   /* Memory at [Register] <- 8-bit immediate (e.g. LD (HL), d8) */
    AM_MR,      /* Single Memory Register operand (e.g. INC (HL)) */
    AM_A16_R,   /* Memory at [16-bit address] <- Register (e.g. LD (a16), A) */
    AM_R_A16    /* Register <- Memory at [16-bit address] (e.g. LD A, (a16)) */
} addr_mode;

/* Register Identifiers */
typedef enum {
    RT_NONE,
    RT_A,
    RT_F,
    RT_B,
    RT_C,
    RT_D,
    RT_E,
    RT_H,
    RT_L,
    RT_AF,
    RT_BC,
    RT_DE,
    RT_HL,
    RT_SP,
    RT_PC
} reg_type;

/* Instruction Opcodes / Types */
typedef enum {
    IN_NONE,
    IN_NOP,
    IN_LD,
    IN_INC,
    IN_DEC,
    IN_RLCA,
    IN_ADD,
    IN_RRCA,
    IN_STOP,
    IN_RLA,
    IN_JR,
    IN_RRA,
    IN_DAA,
    IN_CPL,
    IN_SCF,
    IN_CCF,
    IN_HALT,
    IN_ADC,
    IN_SUB,
    IN_SBC,
    IN_AND,
    IN_XOR,
    IN_OR,
    IN_CP,
    IN_POP,
    IN_JP,
    IN_PUSH,
    IN_RET,
    IN_CB,
    IN_CALL,
    IN_RETI,
    IN_LDH,
    IN_JPHL,
    IN_DI,
    IN_EI,
    IN_RST,
    IN_ERR,

    /* 0xCB-Prefixed Bitwise Operations */
    IN_RLC,
    IN_RRC,
    IN_RL,
    IN_RR,
    IN_SLA,
    IN_SRA,
    IN_SWAP,
    IN_SRL,
    IN_BIT,
    IN_RES,
    IN_SET
} instr_type;

/* Branch / Jump Condition Codes */
typedef enum {
    CT_NONE,    /* Unconditional */
    CT_NZ,      /* Not Zero (Z flag == 0) */
    CT_Z,       /* Zero (Z flag == 1) */
    CT_NC,      /* Not Carry (C flag == 0) */
    CT_C        /* Carry (C flag == 1) */
} cond_type;

/* Instruction Metadata Descriptor */
typedef struct {
    instr_type type;
    addr_mode mode;
    reg_type reg_1;
    reg_type reg_2;
    cond_type cond;
    u8 param;
} instruction;

instruction *instruction_by_opcode(u8 opcode);
const char *instr_name(instr_type t);

#endif //GBEMU_INSTRUCTIONS_H
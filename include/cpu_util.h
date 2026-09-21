#ifndef GBEMU_CPU_UTIL_H
#define GBEMU_CPU_UTIL_H

#include <common.h>
#include "cpu.h"

u16 cpu_read_reg(reg_type rt);
void cpu_set_reg(reg_type rt, u16 value);

#endif //GBEMU_CPU_UTIL_H
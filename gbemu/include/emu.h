#pragma once

#include <common.h>

typedef struct {
    bool paused;
    bool running;
    u64 ticks;
} emu_context;

bool emu_init_embedded(size_t rom_start, size_t rom_size);
bool emu_init_desktop(int argc, char **argv);
int emu_run(void);

emu_context *emu_get_context();

void emu_cycle(int cpu_cycles);

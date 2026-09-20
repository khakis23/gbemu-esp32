#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/* 
  Emu components:

  |Cart|
  |CPU|
  |Address Bus|
  |PPU|
  |Timer|

*/

static emu_context emu_ctx;

emu_context *emu_get_context() {
    return &emu_ctx;
}

void delay(u32 ms) {
    SDL_Delay(ms);
}

int emu_run(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: emu <rom_file>\n");
        return -1;
    }

    if (!cart_load(argv[1])) {
        printf("Failed to load ROM file: %s\n", argv[1]);
        return -2;
    }

    printf("Cart loaded..\n");

    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INIT\n");
    TTF_Init();
    printf("TTF INIT\n");

    cpu_init();
    
    emu_ctx.running = true;
    emu_ctx.paused = false;
    emu_ctx.ticks = 0;

    while(emu_ctx.running) {
        if (emu_ctx.paused) {
            delay(10);
            continue;
        }

        if (!cpu_step()) {
            printf("CPU Stopped\n");
            return -3;
        }

        // emu_ctx.ticks++;  USING emu_cycle() instead.
    }

    return 0;
}

void emu_cycle(const int cpu_cycles) {
    emu_ctx.ticks += cpu_cycles;
}

#include <stdio.h>
#include <emu.h>
#include <cart.h>
#include <cpu.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

/*
  The EMU serves 2 roles:
    1. The application runner: init, core EMU loop.
    2. The "motherboard" or core time keeper.


  EMU components:

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

bool emu_init_embedded(size_t rom_start, size_t rom_size) {
    // TODO Init Display Driver Here...

    return cart_load_buffer((u8 *)rom_start, rom_size);
}

bool emu_init_desktop(int argc, char **argv) {
    if (argc > 2) {
        printf("Usage: ./gbemu <path-to-rom>");
        return false;
    }

    // Init SDL
    SDL_Init(SDL_INIT_VIDEO);
    printf("SDL INIT\n");
    TTF_Init();
    printf("TTF INIT\n");

    return cart_load_disk(argv[1]);
}

int emu_run(void) {
    printf("Cart loaded...\n");

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

        // emu_ctx.ticks++; REMINDER: using emu_cycle() instead.
    }
    return 0;
}

void emu_cycle(const int cpu_cycles) {
    emu_ctx.ticks += cpu_cycles;
}

#if defined(PLATFORM_ESP32) || defined(ESP_PLATFORM)
#include <stddef.h>

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <emu.h>

// TESTING: embedded ROM files
extern const uint8_t rom_start[] asm("_binary_rom_gb_start");
extern const uint8_t rom_end[]   asm("_binary_rom_gb_end");

static const char *TAG = "GBEMU_ESP32";

void app_main(void) {
    size_t rom_size = rom_end - rom_start;
    ESP_LOGI("GBEMU", "Loaded embedded ROM (%d bytes)", rom_size);

    /* Embedded hardware initialization, display driver, and emulation task */
    if (emu_run_embedded(rom_start, rom_size))
        return emu_run():
    return 1;
}

#else

/* Desktop macOS / Debug Host Entry Point with SDL2 */
#include <emu.h>

int main(int argc, char **argv) {
    if (emu_init_desktop(argc, argv))
        return emu_run();
    return 1;
}

#endif

#if defined(PLATFORM_ESP32) || defined(ESP_PLATFORM)

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <emu.h>

static const char *TAG = "GBEMU_ESP32";

void app_main(void) {
    ESP_LOGI(TAG, "Starting Game Boy Emulator on ESP32...");

    /* Embedded hardware initialization, display driver, and emulation task */
}

#else

/* Desktop macOS / Debug Host Entry Point with SDL2 */
#include <emu.h>

int main(int argc, char **argv) {
    return emu_run(argc, argv);
}

#endif

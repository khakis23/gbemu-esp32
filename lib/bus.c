#include "bus.h"
#include <cart.h>


/*
+-------------------------------------------------------------------------+
|                           16-Bit System Bus                             |
|  $0000-$7FFF  Cartridge ROM (Bank 0 & Switchable Bank 1)                |
|  $8000-$9FFF  VRAM (Video RAM - Tile Data & Background Maps)            |
|  $A000-$BFFF  External Cartridge RAM (Save Data)                        |
|  $C000-$DFFF  WRAM (Work RAM)                                           |
|  $E000-$FDFF  Echo RAM (Mirror of WRAM)                                 |
|  $FE00-$FE9F  OAM (Sprite Attribute Table)                              |
|  $FF00-$FF7F  I/O Registers (PPU, Timers, Joypad, Audio)                |
|  $FF80-$FFFE  HRAM (High RAM / Zero Page)                               |
|  $FFFF        IE (Interrupt Enable Register)                            |
+-------------------------------------------------------------------------+
 */

/* Requires 1 clock cycle */
u8 bus_read(const u16 address) {
    // Cartridge ROM
    if (address < 0x8000) {
        return cart_read(address);
    }

    printf("Unsupported read from address: %04X\n", address);
    return 0;
}

/* Requires 1 clock cycle */
void bus_write(const u16 address, const u8 value) {
    // Cartridge ROM
    if (address < 0x8000) {
        return cart_write(address, value);
    }

    printf("Unsupported write to address: %04X\n", address);
}

/* Requires **2** clock cycles */
u16 bus_read16(const u16 address) {
    const u8 lo = bus_read(address);
    const u8 hi = bus_read(address + 1);
    return (hi << 8) | lo;
}

/* Requires **2** clock cycles */
void bus_write16(const u16 address, const u16 value) {
    bus_write(address, value & 0xFF);
    bus_write(address + 1, value >> 8);
}

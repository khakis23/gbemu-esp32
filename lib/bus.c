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


u8 bus_read(u16 address) {
    // Cartridge ROM
    if (address < 0x8000) {
        return cart_read(address);
    }

    // TODO
    NO_IMPL
}

void bus_write(u16 address, u8 value) {
    // Cartridge ROM
    if (address < 0x8000) {
        return cart_write(address, value);
    }

    // TODO
    NO_IMPL
}


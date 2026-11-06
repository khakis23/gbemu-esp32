
## Build Instructions

### ESP32

Right now, the ROM is embedded into the device's RAM. Change `set(SELECTED_ROM "tetris.gb")`
in `main/CMakeLists.txt` to select a different ROM.

#### Build
```bash
. ~/esp/esp-idf/export.sh
idf.py build
```
#### Flash and Run Serial Monitor
```bash
idf.py flash monitor
```

#### Find USB device in MacOS
```bash
ls /dev/cu.usb*
```

### MacOS

#### Build
```bash
cmake -B build-desktop -S .
cmake --build build-desktop
```
#### Run
```bash
# ./gbmeu <path-to-rom>
./build-desktop/main/gbemu roms/tetris.gb
```

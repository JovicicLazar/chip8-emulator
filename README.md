# CHIP-8 Emulator

A simple CHIP-8 emulator written in C++ using [raylib](https://www.raylib.com/) for graphics and input. This emulator loads and runs classic CHIP-8 ROMs with a modern rendering backend.

## Features

- Full CHIP-8 instruction set support
- Display rendered using raylib
- Easy to build with Make

---

## Dependencies

Make sure you have the following installed:

- [raylib](https://www.raylib.com/) (v4.0 or newer recommended)
- `make`
- `g++` (any C++17-compatible compiler)

On Ubuntu/Debian:
```bash
sudo apt install libraylib-dev make g++
```

## Build
```bash
git clone https://github.com/JovicicLazar/chip8-emulator.git
cd chip8-emulator
make
```

## Run 
```bash
./play <Scale> <Delay> <ROM>

```

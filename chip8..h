#ifndef CHIP8_H
#define CHIP8_H

#include <cstdint>
#include <fstream>
#include <chrono>
#include <random>
#include <memory.h>

#include "fonts.h"

const unsigned int START_ADDRESS         = 0x200;
const unsigned int FONTSET_START_ADDRESS = 0x50;
const unsigned int VIDEO_WIDTH           = 64;
const unsigned int VIDEO_HEIGHT          = 32;

class Chip8 {
    public:
        uint8_t delayTimer{};
        uint8_t soundTimer{};
        uint8_t sp{};
        uint8_t registers[ 16 ]{};
        uint8_t memory[ 4096 ]{};
        uint8_t keypad[ 16 ]{};
        uint16_t index{};
        uint16_t pc{};
        uint16_t opcode;
        uint16_t stack[ 16 ]{};
        uint32_t video[ VIDEO_WIDTH * VIDEO_HEIGHT ]{};

        std::default_random_engine randGen;
	    std::uniform_int_distribution<uint8_t> randByte;

        Chip8();

        void LoadROM( char const* filename );

        // CHIP8 operations
        void Chip8::OP_00E0();
        void Chip8::OP_00EE();
        void Chip8::OP_1nnn();
        void Chip8::OP_2nnn();
        void Chip8::OP_3xkk();
        void Chip8::OP_4xkk();
        void Chip8::OP_5xy0();
        void Chip8::OP_6xkk();
        void Chip8::OP_7xkk();
        void Chip8::OP_8xy0();
        void Chip8::OP_8xy1();
        void Chip8::OP_8xy2();
        void Chip8::OP_8xy3();
        void Chip8::OP_8xy4();
        void Chip8::OP_8xy5();
        void Chip8::OP_8xy6();
        void Chip8::OP_8xy7();
        void Chip8::OP_8xyE();
        void Chip8::OP_9xy0();
        void Chip8::OP_Annn();
        void Chip8::OP_Bnnn();
        void Chip8::OP_Cxkk();
        void Chip8::OP_Dxyn();
        void Chip8::OP_Ex9E();
        void Chip8::OP_ExA1();
        void Chip8::OP_Fx07();
        void Chip8::OP_Fx0A();
        void Chip8::OP_Fx15();
        void Chip8::OP_Fx18();
        void Chip8::OP_Fx1E();
        void Chip8::OP_Fx29();
        void Chip8::OP_Fx33();
        void Chip8::OP_Fx55();
        void Chip8::OP_Fx65();
};

#endif
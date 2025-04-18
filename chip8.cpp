#include "chip8.hpp"

Chip8::Chip8()
    : randGen(std::chrono::system_clock::now().time_since_epoch().count()) {
    this->pc = START_ADDRESS;

    for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
        this->memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    this->randByte = std::uniform_int_distribution<uint8_t>(0, 255U);
}

void Chip8::LoadROM(char const *filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open()) {
        std::streampos size = file.tellg();
        char *buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (long i = 0; i < size; ++i) {
            this->memory[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;
    }
}

// nnn or addr - A 12-bit value, the lowest 12 bits of the instruction
// n or nibble - A 4-bit value, the lowest 4 bits of the instruction
// x           - A 4-bit value, the lower 4 bits of the high byte of the
// instruction y           - A 4-bit value, the upper 4 bits of the low byte of
// the instruction kk or byte  - An 8-bit value, the lowest 8 bits of the
// instruction

// CHIP8 operations
void Chip8::OP_00E0() { memset(this->video, 0, sizeof(this->video)); }

void Chip8::OP_00EE() {
    --this->sp;
    this->pc = this->stack[this->sp];
}

void Chip8::OP_1nnn() {
    uint16_t address = this->opcode & 0x0FFFu;
    this->pc = address;
}

void Chip8::OP_2nnn() {
    uint16_t address = this->opcode & 0x0FFFu;
    this->stack[this->sp] = this->pc;
    ++this->sp;
    this->pc = address;
}

void Chip8::OP_3xkk() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t byte = this->opcode & 0x00FFu;

    if (this->registers[Vx] == byte) {
        this->pc += 2;
    }
}

void Chip8::OP_4xkk() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t byte = this->opcode & 0x00FFu;

    if (this->registers[Vx] == byte) {
        this->pc += 2;
    }
}

void Chip8::OP_5xy0() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;

    if (this->registers[Vx] == this->registers[Vy]) {
        pc += 2;
    }
}

void Chip8::OP_6xkk() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t byte = this->opcode & 0x00FFu;
    this->registers[Vx] = byte;
}

void Chip8::OP_7xkk() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t byte = this->opcode & 0x00FFu;
    this->registers[Vx] += byte;
}

void Chip8::OP_8xy0() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;
    this->registers[Vx] = this->registers[Vy];
}

void Chip8::OP_8xy1() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;
    this->registers[Vx] |= this->registers[Vy];
}

void Chip8::OP_8xy2() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;
    this->registers[Vx] &= this->registers[Vy];
}

void Chip8::OP_8xy3() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;
    this->registers[Vx] ^= this->registers[Vy];
}

void Chip8::OP_8xy4() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;
    uint16_t sum = this->registers[Vx] + this->registers[Vy];

    if (sum > 255U) {
        this->registers[0xF] = 1;
    } else {
        this->registers[0xF] = 0;
    }

    this->registers[Vx] = sum & 0xFFu;
}

void Chip8::OP_8xy5() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;

    if (this->registers[Vx] > this->registers[Vy]) {
        this->registers[0xF] = 1;
    } else {
        this->registers[0xF] = 0;
    }

    this->registers[Vx] -= this->registers[Vy];
}

void Chip8::OP_8xy6() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    this->registers[0xF] = (this->registers[Vx] & 0x1u);
    this->registers[Vx] >>= 1;
}

void Chip8::OP_8xy7() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;

    if (this->registers[Vy] > this->registers[Vx]) {
        this->registers[0xF] = 1;
    } else {
        this->registers[0xF] = 0;
    }

    this->registers[Vx] = this->registers[Vy] - this->registers[Vx];
}

void Chip8::OP_8xyE() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    this->registers[0xF] = (this->registers[Vx] & 0x80u) >> 7u;
    this->registers[Vx] <<= 1;
}

void Chip8::OP_9xy0() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;

    if (this->registers[Vx] != this->registers[Vy]) {
        this->pc += 2;
    }
}

void Chip8::OP_Annn() {
    uint16_t address = this->opcode & 0x0FFFu;
    this->index = address;
}

void Chip8::OP_Bnnn() {
    uint16_t address = opcode & 0x0FFFu;
    this->pc = this->registers[0] + address;
}

void Chip8::OP_Cxkk() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t byte = this->opcode & 0x00FFu;
    this->registers[Vx] = this->randByte(randGen) & byte;
}

void Chip8::OP_Dxyn() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (this->opcode & 0x00F0u) >> 4u;
    uint8_t height = this->opcode & 0x000Fu;
    uint8_t xPos = this->registers[Vx] %
                   VIDEO_WIDTH; // wrap x if it goes out of the screen
    uint8_t yPos = this->registers[Vy] %
                   VIDEO_HEIGHT; // wrap y if it goes out of the screen
    this->registers[0xF] = 0;

    for (unsigned int row = 0; row < height; ++row) {
        uint8_t spriteByte = this->memory[this->index + row];

        for (unsigned int col = 0; col < 8; ++col) {
            uint8_t spritePixel = spriteByte & (0x80u >> col);
            uint32_t *screenPixel =
                &this->video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

            if (spritePixel) {
                if (*screenPixel == 0xFFFFFFFF) {
                    registers[0xF] = 1;
                }

                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

void Chip8::OP_Ex9E() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t key = this->registers[Vx];

    if (keypad[key]) {
        pc += 2;
    }
}

void Chip8::OP_ExA1() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t key = this->registers[Vx];

    if (!keypad[key]) {
        pc += 2;
    }
}

void Chip8::OP_Fx07() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    this->registers[Vx] = this->delayTimer;
}

void Chip8::OP_Fx0A() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;

    if (this->keypad[0]) {
        this->registers[Vx] = 0;
    } else if (this->keypad[1]) {
        this->registers[Vx] = 1;
    } else if (this->keypad[2]) {
        this->registers[Vx] = 2;
    } else if (this->keypad[3]) {
        this->registers[Vx] = 3;
    } else if (this->keypad[4]) {
        this->registers[Vx] = 4;
    } else if (this->keypad[5]) {
        this->registers[Vx] = 5;
    } else if (this->keypad[6]) {
        this->registers[Vx] = 6;
    } else if (this->keypad[7]) {
        this->registers[Vx] = 7;
    } else if (this->keypad[8]) {
        this->registers[Vx] = 8;
    } else if (this->keypad[9]) {
        this->registers[Vx] = 9;
    } else if (this->keypad[10]) {
        this->registers[Vx] = 10;
    } else if (this->keypad[11]) {
        this->registers[Vx] = 11;
    } else if (this->keypad[12]) {
        this->registers[Vx] = 12;
    } else if (this->keypad[13]) {
        this->registers[Vx] = 13;
    } else if (this->keypad[14]) {
        this->registers[Vx] = 14;
    } else if (this->keypad[15]) {
        this->registers[Vx] = 15;
    } else {
        this->pc -= 2;
    }
}

void Chip8::OP_Fx15() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    this->delayTimer = this->registers[Vx];
}

void Chip8::OP_Fx18() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    this->soundTimer = this->registers[Vx];
}

void Chip8::OP_Fx1E() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    this->index += this->registers[Vx];
}

void Chip8::OP_Fx29() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t digit = this->registers[Vx];
    this->index = FONTSET_START_ADDRESS + (5 * digit);
}

void Chip8::OP_Fx33() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;
    uint8_t value = this->registers[Vx];
    this->memory[this->index + 2] = value % 10;
    value /= 10;
    this->memory[this->index + 1] = value % 10;
    value /= 10;
    this->memory[this->index] = value % 10;
}

void Chip8::OP_Fx55() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;

    for (uint8_t i = 0; i <= Vx; ++i) {
        this->memory[this->index + i] = registers[i];
    }
}

void Chip8::OP_Fx65() {
    uint8_t Vx = (this->opcode & 0x0F00u) >> 8u;

    for (uint8_t i = 0; i <= Vx; ++i) {
        this->registers[i] = this->memory[this->index + i];
    }
}

#include "Chip8.h"
#include <fstream>
#include <iostream>
#include <random>
#include "../Logger/Logger.h"

const unsigned int START_ADDRESS = 0x200;
const unsigned int FONT_START_ADDRESS = 0x050;
const unsigned int FONTSET_SIZE = 80;
const uint8_t FONTSET[FONTSET_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::LoadROM(const std::string &path) {
    // Load ROM file
    std::ifstream rom(path, std::ios::binary | std::ios::ate);
    if (rom.fail() || !rom.is_open()) {
        ERROR("Failed to open ROM: " + path);
        throw std::ios::failure(std::strerror(errno));
    }

    // Place into memory at START_ADDRESS
    std::streampos size = rom.tellg();
    char *buffer = new char[size];
    rom.seekg(0, std::ios::beg);
    rom.read(buffer, size);
    rom.close();

    for (size_t i = 0; i <= size; ++i) {
        memory[START_ADDRESS + i] = buffer[i];
    }
}

uint16_t Chip8::Fetch() {
    // Read instruction from PC
    uint16_t opcode = (memory[pc] << 8) | memory[pc + 1];
    DEBUG("Opcode: " + fmt::format("{:X}", opcode));

    // Increment PC
    pc += 2;

    return opcode;
}

void Chip8::Execute(uint16_t opcode) {
    // grab first 4 bits from opcode
    switch ((opcode & 0xF000u) >> 12) {
        case 0x0:
            switch ((opcode & 0x0FFFu)) {
                case 0x0E0:
                    Op_00E0();
                    break;
                case 0x0EE:
                    Op_00EE();
                    break;
                default:
                    DecodeFailed(opcode);
            }
            break;
        case 0x1:
            Op_1NNN(opcode);
            break;
        case 0x2:
            Op_2NNN(opcode);
            break;
        case 0x3:
            Op_3XNN(opcode);
            break;
        case 0x4:
            Op_4XNN(opcode);
            break;
        case 0x5:
            Op_5XY0(opcode);
            break;
        case 0x6:
            Op_6XNN(opcode);
            break;
        case 0x7:
            Op_7XNN(opcode);
            break;
        case 0x8:
            switch (opcode & 0x000Fu) {
                case 0x0:
                    Op_8XY0(opcode);
                    break;
                case 0x1:
                    Op_8XY1(opcode);
                    break;
                case 0x2:
                    Op_8XY2(opcode);
                    break;
                case 0x3:
                    Op_8XY3(opcode);
                    break;
                case 0x4:
                    Op_8XY4(opcode);
                    break;
                case 0x5:
                    Op_8XY5(opcode);
                    break;
                case 0x6:
                    Op_8XY6(opcode);
                    break;
                case 0x7:
                    Op_8XY7(opcode);
                    break;
                case 0xE:
                    Op_8XYE(opcode);
                    break;
                default:
                    DecodeFailed(opcode);
            }
            break;
        case 0x9:
            Op_9XY0(opcode);
            break;
        case 0xA:
            Op_ANNN(opcode);
            break;
        case 0xB:
            Op_BNNN(opcode);
            break;
        case 0xC:
            Op_CXNN(opcode);
            break;
        case 0xD:
            Op_DXYN(opcode);
            break;
        case 0xF:
            switch (opcode & 0x00FFu) {
                case 0x07:
                    Op_FX07(opcode);
                    break;
                case 0x0A:
                    Op_FX0A(opcode);
                    break;
                case 0x15:
                    Op_FX15(opcode);
                    break;
                case 0x18:
                    Op_FX18(opcode);
                    break;
                case 0x1E:
                    Op_FX1E(opcode);
                    break;
                case 0x29:
                    Op_FX29(opcode);
                    break;
                case 0x33:
                    Op_FX33(opcode);
                    break;
                case 0x55:
                    Op_FX55(opcode);
                    break;
                case 0x65:
                    Op_FX65(opcode);
                    break;
                default:
                    DecodeFailed(opcode);
            }
        default:
            DecodeFailed(opcode);
    }
}

void Chip8::DecodeFailed(uint16_t opcode) {
    WARN("Opcode cannot be decoded: " + fmt::format("{:X}", opcode));
    errorCodes.push_back(opcode);
}

void Chip8::Op_00E0() {
    // Clear screen
    memset(display, 0, sizeof(display));
}

void Chip8::Op_00EE() {
    // Return from a subroutine
    // Pop the last address from the stack
    pc = stack[sp--];
}

void Chip8::Op_1NNN(uint16_t opcode) {
    // Set PC to NNN
    pc = opcode & 0x0FFFu;
}

void Chip8::Op_2NNN(uint16_t opcode) {
    // Call the subroutine at location NNN
    stack[++sp] = pc;
    pc = opcode & 0x0FFFu;
}

void Chip8::Op_3XNN(uint16_t opcode) {
    // Skip one instruction if VX == NN
    pc = registers[(opcode & 0x0F00u) >> 8] == (uint8_t) opcode
         ? pc + 2 : pc;
}

void Chip8::Op_4XNN(uint16_t opcode) {
    // Skip one instruction if VX != NN
    pc = registers[(opcode & 0x0F00u) >> 8] == (uint8_t) opcode
         ? pc : pc + 2;
}

void Chip8::Op_5XY0(uint16_t opcode) {
    // Skip one instruction if VX == VY
    pc = registers[(opcode & 0x0F00u) >> 8] == registers[(opcode & 0x00F0u) >> 4]
         ? pc + 2 : pc;
}

void Chip8::Op_6XNN(uint16_t opcode) {
    // Set register VX to NN
    registers[(opcode & 0x0F00u) >> 8] = (uint8_t) opcode;
}

void Chip8::Op_7XNN(uint16_t opcode) {
    // Add NN to register VX
    registers[(opcode & 0x0F00u) >> 8] += (uint8_t) opcode;
}

void Chip8::Op_8XY0(uint16_t opcode) {
    // Set VX = VY
    registers[(opcode & 0x0F00u) >> 8] = registers[(opcode & 0x00F0u) >> 4];
}

void Chip8::Op_8XY1(uint16_t opcode) {
    // Set VX |= VY
    registers[(opcode & 0x0F00u) >> 8] |= registers[(opcode & 0x00F0u) >> 4];
}

void Chip8::Op_8XY2(uint16_t opcode) {
    // Set VX &= VY
    registers[(opcode & 0x0F00u) >> 8] &= registers[(opcode & 0x00F0u) >> 4];
}

void Chip8::Op_8XY3(uint16_t opcode) {
    // Set VX ^= VY
    registers[(opcode & 0x0F00u) >> 8] ^= registers[(opcode & 0x00F0u) >> 4];
}

void Chip8::Op_8XY4(uint16_t opcode) {
    // Set VX += VY
    registers[(opcode & 0x0F00u) >> 8] + registers[(opcode & 0x00F0u) >> 4] > 255
    ? registers[0xF] = 1 : registers[0xF] = 0;
    registers[(opcode & 0x0F00u) >> 8] += registers[(opcode & 0x00F0u) >> 4];
}

void Chip8::Op_8XY5(uint16_t opcode) {
    // Set VX = VX - VY
    if (registers[(opcode & 0x0F00u) >> 8] > registers[(opcode & 0x00F0u) >> 4]) {
        registers[0xF] = 1;
    } else {
        registers[0xF] = 0;
    }
    registers[(opcode & 0x0F00u) >> 8] -= registers[(opcode & 0x00F0u) >> 4];
}

void Chip8::Op_8XY6(uint16_t opcode) {
    // SUPER-CHIP shift - ignore Y
    // Shift VX one bit to the right

    // Set VF to the bit that will be shifted out
    registers[0xF] = registers[(opcode & 0x0F00) >> 8] & 0x01u;

    registers[(opcode & 0x0F00) >> 8] >>= 1;
}

void Chip8::Op_8XY7(uint16_t opcode) {
    // Set VX = VY - VX
    if (registers[(opcode & 0x00F0u) >> 4] > registers[(opcode & 0x0F00u) >> 8]) {
        registers[0xF] = 1;
    } else {
        registers[0xF] = 0;
    }
    registers[(opcode & 0x0F00u) >> 8] = registers[(opcode & 0x00F0u) >> 4] - registers[(opcode & 0x0F00u) >> 8];
}

void Chip8::Op_8XYE(uint16_t opcode) {
    // SUPER-CHIP shift - ignore Y
    // Shift VX one bit to the left

    // Set VF to the bit that will be shifted out
    registers[0xF] = (registers[(opcode & 0x0F00) >> 8] & 0x80u) >> 8;

    registers[(opcode & 0x0F00) >> 8] <<= 1;
}

void Chip8::Op_9XY0(uint16_t opcode) {
    // Skip one instruction if VX != VY
    pc = registers[(opcode & 0x0F00u) >> 8] == registers[(opcode & 0x00F0u) >> 4]
         ? pc : pc + 2;
}

void Chip8::Op_ANNN(uint16_t opcode) {
    // Set register I to NNN
    index = opcode & 0x0FFFu;
}

void Chip8::Op_BNNN(uint16_t opcode) {
    // Jump to location NNN plus the value in V0
    pc = registers[0x0] + (opcode & 0x0FFF);
}

void Chip8::Op_CXNN(uint16_t opcode) {
    // Generates a random number, &'s it with NN, and stores the result in VX
    registers[(opcode & 0x0F00u) >> 8] = random(rng) & (uint8_t) opcode;
}

void Chip8::Op_EX9E(uint16_t opcode) {
    // Skips an instruction if the key corresponding to the value in VX is pressed
    // TODO - Input handling
}

void Chip8::Op_EXA1(uint16_t opcode) {
    // Skips an instruction if the key corresponding to the value in VX is not pressed
    // TODO - Input handling
}

void Chip8::Op_FX07(uint16_t opcode) {
    // Set VX to the value in the delay timer
    registers[(opcode & 0x0F00u) >> 8] = delayTimer;
}

void Chip8::Op_FX15(uint16_t opcode) {
    // Set the delay timer to VX
    delayTimer = registers[(opcode & 0x0F00u) >> 8];
}

void Chip8::Op_FX18(uint16_t opcode) {
    // Set the sound timer to VX
    soundTimer = registers[(opcode & 0x0F00u) >> 8];
}

void Chip8::Op_FX1E(uint16_t opcode) {
    // Set I += VX
    // TODO
}

void Chip8::Op_FX0A(uint16_t opcode) {
    // Stop executing instructions and wait for key loop
    // TODO
}

void Chip8::Op_FX29(uint16_t opcode) {
    // Set I to the address of the hex character in VX
    // TODO
}

void Chip8::Op_FX33(uint16_t opcode) {
    // Takes the number in VX and converts it to three decimal digits
    // TODO
}

void Chip8::Op_FX55(uint16_t opcode) {
    // Stores registers V0 thorugh VX in memory starting at I
    // TODO
}

void Chip8::Op_FX65(uint16_t opcode) {
    // Reads registers V0 through VX from memory starting at I
    // TODO
}


void Chip8::Op_DXYN(uint16_t opcode) {
    // Draw to display

    uint8_t xCoord = registers[(opcode & 0x0F00u) >> 8u] % 64;
    uint8_t yCoord = registers[(opcode & 0x00F0u) >> 4u] % 32;
    uint8_t height = opcode & 0x000Fu;

    // Clear the flag register
    registers[0xF] = 0;

    for (size_t row = 0; row < height; ++row) {
        // get the n-th byte of data from the address stored in I
        uint8_t byte = memory[index + row];
        for (size_t col = 0; col < 8; ++col) {
            uint8_t spritePixel = byte & (0x80u >> col);
            uint32_t *screenPixel = &display[(yCoord + row) * 64 + (xCoord + col)];
            if (spritePixel) {
                if (*screenPixel == 0xFFFFFFFF) {
                    registers[0xF] = 1;
                }

                *screenPixel ^= 0xFFFFFFFF;
            }
        }
    }
}

Chip8::Chip8() {
    pc = START_ADDRESS;

    // Load font
    for (size_t i = 0; i < FONTSET_SIZE; ++i) {
        memory[FONT_START_ADDRESS + i] = FONTSET[i];
    }

    std::random_device rd{};
    rng.seed(rd());
}

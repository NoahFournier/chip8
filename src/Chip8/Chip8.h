#ifndef CHIP8_CHIP8_H
#define CHIP8_CHIP8_H

#include <cstdint>
#include <string>
#include <vector>
#include <random>

struct Chip8 {

    Chip8();

    void LoadROM(const std::string &path);

    uint16_t Fetch();

    void Execute(uint16_t);

    uint8_t memory[4096]{};
    uint8_t registers[16]{};
    uint16_t index{};
    uint8_t delayTimer{};
    uint8_t soundTimer{};
    uint16_t pc{};
    uint8_t sp{};
    uint16_t stack[16]{};
    uint32_t display[64 * 32]{};
    uint8_t keypad[16]{};

    std::mt19937 rng;
    std::uniform_int_distribution<uint8_t> random;

private:

    void DecodeFailed(uint16_t opcode);

    void Op_00E0();

    void Op_00EE();

    void Op_1NNN(uint16_t opcode);

    void Op_2NNN(uint16_t opcode);

    void Op_6XNN(uint16_t opcode);

    void Op_7XNN(uint16_t opcode);

    void Op_ANNN(uint16_t opcode);

    void Op_DXYN(uint16_t opcode);

    void Op_3XNN(uint16_t opcode);

    void Op_4XNN(uint16_t opcode);

    void Op_5XY0(uint16_t opcode);

    void Op_9XY0(uint16_t opcode);

    void Op_8XY0(uint16_t opcode);

    void Op_8XY1(uint16_t opcode);

    void Op_8XY2(uint16_t opcode);

    void Op_8XY3(uint16_t opcode);

    void Op_8XY4(uint16_t opcode);

    void Op_8XY5(uint16_t opcode);

    void Op_8XY6(uint16_t opcode);

    void Op_8XY7(uint16_t opcode);

    void Op_8XYE(uint16_t opcode);

    void Op_BNNN(uint16_t opcode);

    void Op_CXNN(uint16_t opcode);

    void Op_EX9E(uint16_t opcode);

    void Op_EXA1(uint16_t opcode);

    void Op_FX07(uint16_t opcode);

    void Op_FX0A(uint16_t opcode);

    void Op_FX15(uint16_t opcode);

    void Op_FX18(uint16_t opcode);

    void Op_FX1E(uint16_t opcode);

    void Op_FX29(uint16_t opcode);

    void Op_FX33(uint16_t opcode);

    void Op_FX55(uint16_t opcode);

    void Op_FX65(uint16_t opcode);
};


#endif //CHIP8_CHIP8_H

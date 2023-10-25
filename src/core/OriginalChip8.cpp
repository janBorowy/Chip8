#include "OriginalChip8.h"

OriginalChip8::OriginalChip8(const Chip8Keyboard &keyboard): Chip8(keyboard) {}

void OriginalChip8::shiftRight(uint16_t instruction) {
    auto vy = getYRegister(instruction);
    auto shiftedBit = vy & 0x01;
    setXRegister(instruction, vy >> 1);
    variables[0xF] = shiftedBit;
}

void OriginalChip8::shiftLeft(uint16_t instruction) {
    auto vy = getYRegister(instruction);
    auto shiftedBit = (vy & 0x80) >> 7;
    setXRegister(instruction, vy << 1);
    variables[0xF] = shiftedBit;
}

void OriginalChip8::jumpWithOffset(uint16_t instruction) {
    uint8_t address = instruction & 0x0FFF;
    address += variables[0x0];
    programCounter = address;
}

void OriginalChip8::storeRegistersToMemory(uint16_t instruction) {
    auto x = getXIdx(instruction);
    for(uint8_t i = 0; i <= x; ++i, ++indexPointer) {
        memory[indexPointer] = variables[i];
    }
}

void OriginalChip8::loadRegistersFromMemory(uint16_t instruction) {
    auto x = getXIdx(instruction);
    for(uint8_t i = 0; i <= x; ++i, ++indexPointer) {
        variables[i] = memory[indexPointer];
    }
}
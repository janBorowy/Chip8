#include "SChip.h"

SChip::SChip(const Chip8Keyboard &keyboard): Chip8(keyboard) {}

void SChip::shiftRight(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto shiftedBit = vx & 0x01;
    setXRegister(instruction, vx >> 1);
    variables[0xF] = shiftedBit;
}

void SChip::shiftLeft(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto shiftedBit = (vx & 0x80) >> 7;
    setXRegister(instruction, vx << 1);
    variables[0xF] = shiftedBit;
}

void SChip::jumpWithOffset(uint16_t instruction) {
    uint8_t address = instruction & 0x0FFF;
    address += getXRegister(instruction);
    programCounter = address;
}

void SChip::storeRegistersToMemory(uint16_t instruction) {
    auto x = getXIdx(instruction);
    auto temporaryI = indexPointer;
    for(uint8_t i = 0; i <= x; ++i, ++temporaryI) {
        memory[temporaryI] = variables[i];
    }
}

void SChip::loadRegistersFromMemory(uint16_t instruction) {
    auto x = getXIdx(instruction);
    auto temporaryI = indexPointer;
    for(uint8_t i = 0; i <= x; ++i, ++temporaryI) {
        variables[i] = memory[temporaryI];
    }
}
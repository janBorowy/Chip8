#pragma once
#include "Chip8.h"

class SChip: public Chip8 {
    void shiftRight(uint16_t);
    void shiftLeft(uint16_t);
    void jumpWithOffset(uint16_t);
    void storeRegistersToMemory(uint16_t);
    void loadRegistersFromMemory(uint16_t);
    void binaryOr(uint16_t instruction);
    void binaryAnd(uint16_t instruction);
    void logicalXor(uint16_t instruction);
    public:
    SChip(const Chip8Keyboard &keyboard);
};
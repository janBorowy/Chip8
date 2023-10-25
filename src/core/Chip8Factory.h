#pragma once
#include "Chip8.h"
#include "SChip.h"
#include "OriginalChip8.h"
#include <memory>

enum CHIP8_IMPLEMENTATION {
    ORIGINAL_CHIP8,
    SCHIP
};

class Chip8Factory {
    public:

    static std::unique_ptr<Chip8> make(CHIP8_IMPLEMENTATION impl,
        const Chip8Keyboard &keyboard);
};
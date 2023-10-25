#include "Chip8Factory.h"

std::unique_ptr<Chip8> Chip8Factory::make(CHIP8_IMPLEMENTATION impl,
    const Chip8Keyboard &keyboard) {
    switch(impl) {
        case ORIGINAL_CHIP8:
            return std::unique_ptr<Chip8>(new OriginalChip8(keyboard));
        case SCHIP:
            return std::unique_ptr<Chip8>(new SChip(keyboard));
    }
}
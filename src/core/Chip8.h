#pragma once
#include <cstdint>
#include <stack>
#include <vector>
#include "Display.h"
#include <memory>

constexpr unsigned int CHIP8_DISPLAY_WIDTH = 64;
constexpr unsigned int CHIP8_DISPLAY_HEIGTH = 32;
constexpr unsigned int CHIP8_PROGRAM_BEGINNING_ADDRESS = 0x200;
constexpr unsigned int CHIP8_TIMER_FREQUENCY = 60;
constexpr unsigned int CHIP8_FONT_MEMORY_LENGTH = 80;
constexpr unsigned int CHIP8_FONT_BEGINNING_ADDRES = 0x50;
constexpr unsigned int CHIP8_MEMORY_SIZE = 4096;
constexpr unsigned int CHIP8_MAX_PROGRAM_SIZE = 
    CHIP8_MEMORY_SIZE - CHIP8_PROGRAM_BEGINNING_ADDRESS;

class Chip8 {

    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint16_t programCounter;
    uint16_t indexPointer;
    std::stack<uint16_t> stack;
    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t variables[16];

    typedef void(Chip8::*InstructionHandler)(uint16_t);

    std::unique_ptr<Display> display;

    uint8_t font[CHIP8_FONT_MEMORY_LENGTH] = {
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

    void initializeVariables();
    void loadFont();
    void startTimers();
    uint16_t fetchInstruction();
    void execute(uint16_t instruction);
    int getXIdx(uint16_t instruction);
    int getYIdx(uint16_t instruction);
    uint8_t getXRegister(uint16_t instruction);
    uint8_t getYRegister(uint16_t instruction);
    int getHandlerIdx(uint16_t instruction);
    std::vector<uint8_t> loadSprite(int height);

    void zeroCategoryHandler(uint16_t instruction);
    void jump(uint16_t instruction);
    void setXRegister(uint16_t instruction);
    void addXRegister(uint16_t instruction);
    void setIndex(uint16_t instruction);
    void draw(uint16_t instruction);

    void clearScreen();

    static constexpr std::array<InstructionHandler, 16> handlers {
        &Chip8::zeroCategoryHandler,
        &Chip8::jump,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        &Chip8::setXRegister,
        &Chip8::addXRegister,
        nullptr,
        nullptr,
        &Chip8::setIndex,
        nullptr,
        nullptr,
        &Chip8::draw,
        nullptr,
        nullptr,
    };

    public:
        Chip8();
        void doNextCycle();
        void loadRom(std::array<char, CHIP8_MAX_PROGRAM_SIZE> data);
        PixelMatrix peek();
};
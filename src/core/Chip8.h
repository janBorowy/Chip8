#pragma once
#include <cstdint>
#include <stack>
#include <vector>
#include "Display.h"
#include <memory>
#include <stdexcept>
#include <random>
#include <chrono>
#include "Timer.h"

constexpr unsigned int CHIP8_DISPLAY_WIDTH = 64;
constexpr unsigned int CHIP8_DISPLAY_HEIGTH = 32;
constexpr unsigned int CHIP8_PROGRAM_BEGINNING_ADDRESS = 0x200;
constexpr unsigned int CHIP8_TIMER_FREQUENCY = 60;
constexpr unsigned int CHIP8_FONT_MEMORY_LENGTH = 80;
constexpr unsigned int CHIP8_FONT_BEGINNING_ADDRES = 0x50;
constexpr unsigned int CHIP8_MEMORY_SIZE = 4096;
constexpr unsigned int CHIP8_MAX_PROGRAM_SIZE = 
    CHIP8_MEMORY_SIZE - CHIP8_PROGRAM_BEGINNING_ADDRESS;

class InstructionNotImplemented : public std::runtime_error {
    uint16_t opcode;
    public:
        InstructionNotImplemented(uint16_t opcode): runtime_error("Instruction has not been yet implemented") {
            this->opcode = opcode;
        }
        uint16_t getOpcode() {
            return opcode;
        }
};

class Chip8 {

    uint8_t memory[CHIP8_MEMORY_SIZE];
    uint16_t programCounter;
    uint16_t indexPointer;
    std::stack<uint16_t> stack;
    uint8_t variables[16];

    Timer delayTimer;
    Timer soundTimer;

    std::mt19937 randomEngine;

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
    uint16_t fetchInstruction();
    void execute(uint16_t instruction);
    int getXIdx(uint16_t instruction);
    int getYIdx(uint16_t instruction);
    uint8_t &getXRegister(uint16_t instruction);
    uint8_t &getYRegister(uint16_t instruction);
    uint8_t getXRegisterValue(uint16_t instruction);
    uint8_t getYRegisterValue(uint16_t instruction);
    int getHandlerIdx(uint16_t instruction);
    std::vector<uint8_t> loadSprite(int height);

    void zeroCategoryHandler(uint16_t instruction);
    void jump(uint16_t instruction);
    void callASubroutine(uint16_t instruction);
    void skipEqualLiteral(uint16_t instruction);
    void skipNotEqualLietral(uint16_t instruction);
    void skipEqualRegisters(uint16_t instruction);
    void skipNotEqualRegisters(uint16_t instruction);
    void eightCategoryHandler(uint16_t instruction);
    void setXRegister(uint16_t instruction);
    void addXRegister(uint16_t instruction);
    void setIndex(uint16_t instruction);
    void jumpWithOffset(uint16_t instruction);
    void getRandomNumber(uint16_t instruction);
    void draw(uint16_t instruction);
    void fCategoryHandler(uint16_t instruction);

    void clearScreen();
    void returnFromSubroutine();

    void set(uint16_t instruction);
    void binaryOr(uint16_t instruction);
    void binaryAnd(uint16_t instruction);
    void logicalXor(uint16_t instruction);
    void add(uint16_t instruction);
    void substract(uint16_t instruction);
    void substractInverted(uint16_t instruction);
    void shiftRight(uint16_t instruction);
    void shiftLeft(uint16_t instruction);

    void setVxToDelayTimer(uint16_t instruction);
    void setDelayTimer(uint16_t instruction);
    void setSoundTimer(uint16_t instruction);

    static constexpr std::array<InstructionHandler, 16> handlers {
        &Chip8::zeroCategoryHandler,
        &Chip8::jump,
        &Chip8::callASubroutine,
        &Chip8::skipEqualLiteral,
        &Chip8::skipNotEqualLietral,
        &Chip8::skipEqualRegisters,
        &Chip8::setXRegister,
        &Chip8::addXRegister,
        &Chip8::eightCategoryHandler,
        &Chip8::skipNotEqualRegisters,
        &Chip8::setIndex,
        &Chip8::jumpWithOffset,
        &Chip8::getRandomNumber,
        &Chip8::draw,
        nullptr,
        &Chip8::fCategoryHandler,
    };

    public:
        Chip8();
        void doNextCycle();
        void loadRom(std::array<char, CHIP8_MAX_PROGRAM_SIZE> data);
        PixelMatrix peek();
};
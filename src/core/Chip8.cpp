#include "Chip8.h"
#include <cstring>


Chip8::Chip8() {
    initializeVariables();
    loadFont();
    startTimers();
}

void Chip8::initializeVariables() {
    memset(memory, 0, sizeof(memory));
    memset(variables, 0, sizeof(variables));
    programCounter = CHIP8_PROGRAM_BEGINNING_ADDRESS;
    indexPointer = 0;
    stack = std::stack<uint16_t>();
    delayTimer = 0;
    soundTimer = 0;

    display = std::make_unique<Display>();
}

void Chip8::loadFont() {
    memcpy(memory + CHIP8_FONT_BEGINNING_ADDRES, font, CHIP8_FONT_MEMORY_LENGTH);
}

void Chip8::loadRom(std::array<char, CHIP8_MAX_PROGRAM_SIZE> data) {
    memcpy(memory + CHIP8_PROGRAM_BEGINNING_ADDRESS, data.data(), data.size());
}

void Chip8::doNextCycle() {
    auto instruction = fetchInstruction();
    programCounter = programCounter + 2;
    execute(instruction);
}

void Chip8::execute(uint16_t instruction) {
    int handlerIdx = getHandlerIdx(instruction);
    (this->*handlers[handlerIdx])(instruction);
}

void Chip8::startTimers() {

}

PixelMatrix Chip8::peek() {
    return display->getData();
}

uint16_t Chip8::fetchInstruction () {
    uint8_t firstPart = memory[programCounter];
    uint8_t secondPart = memory[programCounter + 1];
    return ((uint16_t) firstPart << 8) | secondPart;
}

void Chip8::zeroCategoryHandler(uint16_t instruction) {
    if (instruction == 0x00E0) {
        clearScreen();
    }
}

void Chip8::clearScreen() {
    display->clear();
}

void Chip8::jump(uint16_t instruction) {
    int address = instruction & 0x0FFF;
    programCounter = address;
}

void Chip8::setXRegister(uint16_t instruction) {
    int xIdx = getXIdx(instruction);
    variables[xIdx] = instruction & 0x00FF;
}

void Chip8::addXRegister(uint16_t instruction) {
    int xIdx = getXIdx(instruction);
    variables[xIdx] += instruction & 0x00FF;
}

void Chip8::setIndex(uint16_t instruction) {
    int address = instruction & 0x0FFF;
    indexPointer = address;
}

void Chip8::draw(uint16_t instruction) {
    int vx = getXRegister(instruction) % CHIP8_DISPLAY_WIDTH;
    int vy = getYRegister(instruction) % CHIP8_DISPLAY_HEIGTH;
    int height = instruction & 0x000F;
    variables[0xF] = display->drawSprite(vx, vy, loadSprite(height));
}

std::vector<uint8_t> Chip8::loadSprite(int height) {
    std::vector<uint8_t> sprite;
    for(int i = 0; i < height; ++i) {
        sprite.push_back(memory[indexPointer + i]);
    }
    return sprite;
}

int Chip8::getHandlerIdx(uint16_t instruction) {
    return (instruction & 0xF000) >> 12;
}

int Chip8::getXIdx(uint16_t instruction) {
    return (instruction & 0x0F00) >> 8;
}

int Chip8::getYIdx(uint16_t instruction) {
    return (instruction & 0x00F0) >> 4;
}

uint8_t Chip8::getXRegister(uint16_t instruction) {
    int x = getXIdx(instruction);
    return variables[x];
}

uint8_t Chip8::getYRegister(uint16_t instruction) {
    int y = getYIdx(instruction);
    return variables[y];
}
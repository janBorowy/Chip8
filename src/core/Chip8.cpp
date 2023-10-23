#include "Chip8.h"
#include <cstring>


Chip8::Chip8():
    programCounter(CHIP8_PROGRAM_BEGINNING_ADDRESS),
    indexPointer(0),
    stack(),
    delayTimer(),
    soundTimer(),
    awaitingInput(CHIP8_NONE),
    randomEngine(std::chrono::steady_clock::now().time_since_epoch().count()),
    display(std::make_unique<Display>()) {
    initializeVariables();
    loadFont();
}

void Chip8::initializeVariables() {
    memset(memory, 0, sizeof(memory));
    memset(variables, 0, sizeof(variables));
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

    if(handlers[handlerIdx] == nullptr) {
        throw InstructionNotImplemented(instruction);
    }

    (this->*handlers[handlerIdx])(instruction);
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
    } else if(instruction == 0x00EE) {
        returnFromSubroutine();
    }
}

void Chip8::clearScreen() {
    display->clear();
}

void Chip8::returnFromSubroutine() {
    programCounter = stack.top();
    stack.pop();
}

void Chip8::jump(uint16_t instruction) {
    int address = instruction & 0x0FFF;
    programCounter = address;
}

void Chip8::callASubroutine(uint16_t instruction) {
    uint16_t address = instruction & 0x0FFF;
    stack.push(programCounter);
    programCounter = address;
}

void Chip8::skipEqualLiteral(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    uint8_t val = instruction & 0x00FF;
    if(val == vx)
        programCounter += 2;
}

void Chip8::skipEqualRegisters(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto vy = getYRegister(instruction);

    if (vx == vy) {
        programCounter += 2;
    }
}

void Chip8::skipNotEqualLietral(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    uint8_t val = instruction & 0x00FF;
    if(val != vx)
        programCounter += 2;
}

void Chip8::setXRegisterToNN(uint16_t instruction) {
    int xIdx = getXIdx(instruction);
    variables[xIdx] = instruction & 0x00FF;
}

void Chip8::addXRegister(uint16_t instruction) {
    int xIdx = getXIdx(instruction);
    variables[xIdx] += instruction & 0x00FF;
}

void Chip8::skipNotEqualRegisters(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto vy = getYRegister(instruction);
    if(vx != vy) {
        programCounter += 2;
    }
}

void Chip8::eightCategoryHandler(uint16_t instruction) {
    uint8_t selector = instruction & 0x000F;

    switch(selector) {
        case 0x0:
            set(instruction);
            break;
        case 0x1:
            binaryOr(instruction);
            break;
        case 0x2:
            binaryAnd(instruction);
            break;
        case 0x3:
            logicalXor(instruction);
            break;
        case 0x4:
            add(instruction);
            break;
        case 0x5:
            substract(instruction);
            break;
        case 0x6:
            shiftRight(instruction);
            break;
        case 0x7:
            substractInverted(instruction);
            break;
        case 0xE:
            shiftLeft(instruction);
            break;
        default:
            throw InstructionNotImplemented(instruction);
            break;
    }
}

void Chip8::setIndex(uint16_t instruction) {
    int address = instruction & 0x0FFF;
    indexPointer = address;
}

// TODO: ambiguous, make this configurable
void Chip8::jumpWithOffset(uint16_t instruction) {
    uint8_t address = instruction & 0x0FFF;
    address += variables[0x0];
    programCounter = address;
}

void Chip8::getRandomNumber(uint16_t instruction) {
    int randomNumber = randomEngine() % 0xFF;
    int valueToBinaryAnd = instruction & 0x00FF;
    int vx = getXRegister(instruction);
    vx = randomNumber & valueToBinaryAnd;
}

void Chip8::draw(uint16_t instruction) {
    int vx = getXRegister(instruction) % CHIP8_DISPLAY_WIDTH;
    int vy = getYRegister(instruction) % CHIP8_DISPLAY_HEIGTH;
    int height = instruction & 0x000F;
    variables[0xF] = display->drawSprite(vx, vy, loadSprite(height));
}

void Chip8::eCategoryHandler(uint16_t instruction) {
    uint16_t selector = instruction & 0x00FF;    
    switch(selector) {
        case 0x9E:
            skipIfHeld(instruction);
            break;
        case 0xA1:
            skipIfNotHeld(instruction);
            break;
    }
}

void Chip8::skipIfHeld(uint16_t instruction) {
}

void Chip8::skipIfNotHeld(uint16_t instruction) {

}

void Chip8::fCategoryHandler(uint16_t instruction) {
    uint16_t selector = instruction & 0x00FF;

    switch(selector) {
        case 0x07:
            setVxToDelayTimer(instruction);
            break;
        case 0x15:
            setDelayTimer(instruction);
            break;
        case 0x18:
            setSoundTimer(instruction);
            break;
        case 0x1E:
            addToIndex(instruction);
            break;
        case 0x0A:
            getKey(instruction);
            break;
        case 0x29:
            getFontCharacter(instruction);
            break;
        case 0x33:
            binaryCodedDecimalConversion(instruction);
            break;
        case 0x55:
            storeRegistersToMemory(instruction);
            break;
        case 0x65:
            loadRegistersFromMemory(instruction);
            break;
        default:
            throw InstructionNotImplemented(instruction);
            break;
    }
}

void Chip8::setVxToDelayTimer(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    std::chrono::duration<float> timeLeftInSeconds
        = delayTimer.getStatus();
    vx = timeLeftInSeconds.count() * 60;
}

void Chip8::setDelayTimer(uint16_t instruction) {
    auto vxValue = getXRegister(instruction); 
    delayTimer.setTime(std::chrono::milliseconds((vxValue) * 1000 / 60));
}

void Chip8::setSoundTimer(uint16_t instruction) {
    auto vxValue = getXRegister(instruction); 
    soundTimer.setTime(std::chrono::milliseconds((vxValue) * 1000 / 60));
}

void Chip8::addToIndex(uint16_t instruction) {
    auto vxValue = getXRegister(instruction);
    if(indexPointer + vxValue > 0xFFF) {
        variables[0xF] = 1;
    }
    indexPointer += vxValue;
}

void Chip8::getKey(uint16_t instruction) {
    if(awaitingInput != CHIP8_NONE) {
        auto vx = getXRegister(instruction);
        vx = awaitingInput & 0x00FF;
        awaitingInput = CHIP8_NONE;
    }
    programCounter -= 2;
}

void Chip8::sendInput(CHIP8_KEY key) {
    awaitingInput = key;
}

void Chip8::getFontCharacter(uint16_t instruction) {
    auto vxValue = getXRegister(instruction);
    indexPointer = CHIP8_FONT_BEGINNING_ADDRES + 5 * vxValue;
}

void Chip8::binaryCodedDecimalConversion(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    memory[indexPointer] = vx / 100;
    memory[indexPointer + 1] = (vx / 10) % 10;
    memory[indexPointer + 2] = vx % 10;
}

void Chip8::storeRegistersToMemory(uint16_t instruction) {
    auto x = getXIdx(instruction);
    auto temporaryI = indexPointer;
    for(uint8_t i = 0; i <= x; ++i) {
        memory[temporaryI] = variables[i];
        ++temporaryI;
    }
}

void Chip8::loadRegistersFromMemory(uint16_t instruction) {
    auto x = getXIdx(instruction);
    auto temporaryI = indexPointer;
    for(uint8_t i = 0; i <= x; ++i, ++temporaryI) {
        variables[i] = memory[temporaryI];
    }
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

void Chip8::set(uint16_t instruction) {
    auto vy = getYRegister(instruction);
    setXRegister(instruction, vy);
}

void Chip8::binaryOr(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto vyValue = getYRegister(instruction);
    setXRegister(instruction, vx | vyValue);
}

void Chip8::binaryAnd(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto vyValue = getYRegister(instruction);
    setXRegister(instruction, vx & vyValue);
}

void Chip8::logicalXor(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto vyValue = getYRegister(instruction);
    setXRegister(instruction, vx ^ vyValue);
}

void Chip8::add(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto vyValue = getYRegister(instruction);
    if ((int)vx + vyValue > 255) {
        variables[0xF] = 1;
    } else {
        variables[0xF] = 0;
    }
    setXRegister(instruction, vx + vyValue);
}

void Chip8::substract(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto vyValue = getYRegister(instruction);
    if(vx > vyValue) {
        variables[0xF] = 1;
    } else {
        variables[0xF] = 0;
    }
    setXRegister(instruction, vx - vyValue);
}

void Chip8::substractInverted(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    auto vyValue = getYRegister(instruction);
    if(vx < vyValue) {
        variables[0xF] = 1;
    } else {
        variables[0xF] = 0;
    }
    setXRegister(instruction, vyValue - vx);
}

void Chip8::shiftRight(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    variables[0xF] = vx & 0x01;
    setXRegister(instruction, vx >> 1);
}
void Chip8::shiftLeft(uint16_t instruction) {
    auto vx = getXRegister(instruction);
    variables[0xF] = vx & 0x80;
    setXRegister(instruction, vx << 1);
}

void Chip8::setXRegister(uint16_t instruction, uint8_t newValue) {
    auto x = getXIdx(instruction);
    variables[x] = newValue;
}

void Chip8::setYRegister(uint16_t instruction, uint8_t newValue) {
    auto y = getYIdx(instruction);
    variables[y] = newValue;
}

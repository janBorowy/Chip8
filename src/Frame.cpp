#include "Frame.h"
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <thread>
#include <iomanip>
#include <iostream>
#include <SDL2/SDL.h>
#include <memory>


Frame::Frame(std::string romFilePath):
    chip8(std::make_unique<Chip8>(keyboard)),
    shouldQuit(false) {
    tryToInitializeSDL();
    screen = std::make_unique<Screen>(WINDOW_WIDTH, WINDOW_HEIGHT);
    auto romData = loadRomFile(romFilePath);
    chip8->loadRom(romData);
    initializeKeyboard();
}

Frame::~Frame() {
    SDL_Quit();
}

void Frame::tryToInitializeSDL() {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL init error: "
            << SDL_GetError()
            << std::endl;
    }
}


void Frame::startLoop() {
    lastScreenUpdate = Clock::now();
    while(!shouldQuit) {
        auto frameStarted = Clock::now();
        processEventQueue();
        try {
            chip8->doNextCycle();
        } catch (InstructionNotImplemented e) {
            std::cout << "Could not execute instruction: " << std::hex << e.getOpcode() << std::endl;
        }

        if(Clock::now() - lastScreenUpdate
            > std::chrono::duration<float>(1/SCREEN_REFRESH_FREQUENCY)) {
            screen->update(chip8->peek());
        }
        auto sleepDuration = CHIP_CLOCK_PERIOD - (Clock::now() - frameStarted);
        std::this_thread::sleep_for(sleepDuration);
    }
}

Chip8Rom Frame::loadRomFile(std::string filePath) {
    if(determineFileSize(filePath) > CHIP8_MAX_PROGRAM_SIZE) {
        throw RomFileTooLargeException();
    }
    std::ifstream file(filePath);
    if(!file.good()) {
        throw InvalidFileException();
    }
    Chip8Rom data;
    int i = 0;
    while(file.good()) {
        data[i] = file.get();
        ++i;
    }
    file.close();
    return data;
}

long Frame::determineFileSize(std::string filePath) {
    std::ifstream file(filePath, std::ifstream::ate | std::ifstream::binary);
    long size = file.tellg();
    file.close();
    return size;
}

void Frame::processEventQueue() {
    SDL_Event e;
    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            shouldQuit = true;
        } else if(e.type == SDL_KEYDOWN) {
            if(!isChip8Key(e))
                continue;
            keyboard[sdlToChip8KeyMap[e.key.keysym.scancode]] = true;
        } else if(e.type == SDL_KEYUP) {
            if(!isChip8Key(e))
                continue;
            keyboard[sdlToChip8KeyMap[e.key.keysym.scancode]] = false;
        }
    }
}
bool Frame::isChip8Key(const SDL_Event &e) const {
    auto scancode = e.key.keysym.scancode;
    if(sdlToChip8KeyMap.find(scancode) != sdlToChip8KeyMap.end())
        return true;
    return false;
}

void Frame::initializeKeyboard() {
    for(CHIP8_KEY key = CHIP8_0; key < CHIP8_F + 1; key = static_cast<CHIP8_KEY>(key + 1)) {
        keyboard.insert({key, false});
    }
}

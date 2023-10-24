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


Frame::Frame(std::string romFilePath) {
    chip8 = std::make_unique<Chip8>();
    tryToInitializeSDL();
    screen = std::make_unique<Screen>(WINDOW_WIDTH, WINDOW_HEIGHT);
    auto romData = loadRomFile(romFilePath);
    chip8->loadRom(romData);
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
    SDL_Event e;
    bool quit;
    lastScreenUpdate = Clock::now();
    while(!quit) {
        auto frameStarted = Clock::now();
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }
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
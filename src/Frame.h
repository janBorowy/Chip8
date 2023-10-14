#pragma once 
#include "core/Chip8.h"
#include <memory>
#include "Screen.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320
#define SCREEN_REFRESH_FREQUENCY 60
#define CHIP_CLOCK_SPEED 700 //TODO: make this configurable

class RomFileTooLargeException: public std::runtime_error {
    public:
    RomFileTooLargeException():runtime_error("Rom file exceeded max size"){}
};

class Frame {

    std::unique_ptr<Chip8> chip8;
    std::unique_ptr<Screen> screen;

    std::chrono::system_clock::time_point lastScreenUpdate;
    std::chrono::system_clock::time_point lastChipExecution;

    void tryToInitializeSDL();
    std::array<char, CHIP8_MAX_PROGRAM_SIZE> loadRomFile(std::string filePath);
    long determineFileSize(std::string filePath);
    public:
    Frame();
    ~Frame();
    void startLoop();
};
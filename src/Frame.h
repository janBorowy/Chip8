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
#include <thread>
#include <iomanip>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320
#define SCREEN_REFRESH_FREQUENCY 60

class RomFileTooLargeException: public std::runtime_error {
    public:
    RomFileTooLargeException():runtime_error("Rom file exceeded max size"){}
};

class Frame {

    std::unique_ptr<Chip8> chip8;
    std::unique_ptr<Screen> screen;

    typedef std::chrono::steady_clock Clock;

    static auto constexpr CHIP_CLOCK_PERIOD = std::chrono::milliseconds(1/700);

    Clock::time_point lastScreenUpdate;

    void tryToInitializeSDL();
    std::array<char, CHIP8_MAX_PROGRAM_SIZE> loadRomFile(std::string filePath);
    long determineFileSize(std::string filePath);
    public:
    Frame();
    ~Frame();
    void startLoop();
};
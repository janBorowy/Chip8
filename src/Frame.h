#pragma once 
#include "core/Chip8.h"
#include "Screen.h"



#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320
#define SCREEN_REFRESH_FREQUENCY 60

typedef std::array<char, CHIP8_MAX_PROGRAM_SIZE> Chip8Rom;

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
    Chip8Rom loadRomFile(std::string filePath);
    long determineFileSize(std::string filePath);
    public:
    Frame(std::string romFilePath);
    ~Frame();
    void startLoop();
};
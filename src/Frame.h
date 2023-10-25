#pragma once 
#include "core/Chip8.h"
#include "Screen.h"
#include <unordered_map>
#include "core/Chip8Factory.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 320
#define SCREEN_REFRESH_FREQUENCY 60

typedef std::array<char, CHIP8_MAX_PROGRAM_SIZE> Chip8Rom;

class RomFileTooLargeException: public std::runtime_error {
    public:
    RomFileTooLargeException():runtime_error("Rom file exceeded max size"){}
};

class InvalidFileException: public std::runtime_error {
    public:
    InvalidFileException():runtime_error("File does not exist or is corrupted"){}
};

class Frame {

    std::unique_ptr<Chip8> chip8;
    std::unique_ptr<Screen> screen;
    bool shouldQuit;
    typedef std::chrono::steady_clock Clock;
    Chip8Keyboard keyboard;
    std::unordered_map<SDL_Scancode, CHIP8_KEY> sdlToChip8KeyMap = 
    {
        {SDL_SCANCODE_1, CHIP8_1},
        {SDL_SCANCODE_2, CHIP8_2},
        {SDL_SCANCODE_3, CHIP8_3},
        {SDL_SCANCODE_4, CHIP8_C},
        {SDL_SCANCODE_Q, CHIP8_4},
        {SDL_SCANCODE_W, CHIP8_5},
        {SDL_SCANCODE_E, CHIP8_6},
        {SDL_SCANCODE_R, CHIP8_D},
        {SDL_SCANCODE_A, CHIP8_7},
        {SDL_SCANCODE_S, CHIP8_8},
        {SDL_SCANCODE_D, CHIP8_9},
        {SDL_SCANCODE_F, CHIP8_E},
        {SDL_SCANCODE_Z, CHIP8_A},
        {SDL_SCANCODE_X, CHIP8_0},
        {SDL_SCANCODE_C, CHIP8_B},
        {SDL_SCANCODE_V, CHIP8_F},

    };

    static auto constexpr CHIP_CLOCK_PERIOD = std::chrono::seconds(1/700);

    Clock::time_point lastScreenUpdate;

    void tryToInitializeSDL();
    Chip8Rom loadRomFile(std::string filePath);
    long determineFileSize(std::string filePath);
    void processEventQueue();
    void initializeKeyboard();
    bool isChip8Key(const SDL_Event &e) const;
    public:
    Frame(std::string romFilePath,
        CHIP8_IMPLEMENTATION impl = CHIP8_IMPLEMENTATION::ORIGINAL_CHIP8);
    ~Frame();
    void startLoop();
};
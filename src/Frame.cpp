#include "Frame.h"


Frame::Frame() {
    chip8 = std::make_unique<Chip8>();
    tryToInitializeSDL();
    screen = std::make_unique<Screen>(WINDOW_WIDTH, WINDOW_HEIGHT);
    auto romData = loadRomFile("/home/janek/workspace/chip-8-emulator/roms/IBM.ch8");
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
    lastScreenUpdate = std::chrono::system_clock::now();
    lastChipExecution = std::chrono::system_clock::now();
    while(!quit) {
        auto frameStarted = std::chrono::system_clock::now();
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }

        chip8->doNextCycle();

        if(std::chrono::system_clock::now() - lastScreenUpdate
            > std::chrono::duration<float>(1/SCREEN_REFRESH_FREQUENCY)) {
            screen->update(chip8->peek());
        }

        std::this_thread::sleep_for(std::chrono::duration<float>(1/CHIP_CLOCK_SPEED)
            - (std::chrono::system_clock::now() - frameStarted));
    }
}

std::array<char, CHIP8_MAX_PROGRAM_SIZE> Frame::loadRomFile(std::string filePath) {
    if(determineFileSize(filePath) > CHIP8_MAX_PROGRAM_SIZE) {
        throw RomFileTooLargeException();
    }
    std::ifstream file(filePath);
    std::array<char, CHIP8_MAX_PROGRAM_SIZE> data;
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
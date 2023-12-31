#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "core/Chip8.h"


class Screen {

    #define WINDOW_TITLE "Chip-8 emulator"

    SDL_Window *window = nullptr;
    SDL_Surface *surface = nullptr;
    int pixelWidth;
    int pixelHeight;

    void printFailureMessage(const char *message);
    SDL_Window *tryToCreateWindow(int width, int height);
    void drawBackground();
    void drawPixel(int x, int y);
    void calculatePixelDimensions(int screenWidth, int screenHeight);

    public:
    Screen(int initialWidth, int initialHeight);
    ~Screen();
    void update(PixelMatrix pixels);
};
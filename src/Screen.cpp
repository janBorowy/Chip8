#include "Screen.h"

Screen::Screen(int initialWidth, int initialHeight) {
    window = tryToCreateWindow(initialWidth, initialHeight);
    surface = SDL_GetWindowSurface(window);
    calculatePixelDimensions(initialWidth, initialHeight);
}

SDL_Window *Screen::tryToCreateWindow(int width, int height) {
    auto window = SDL_CreateWindow(
        WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_SHOWN);

    if(window == NULL) {
        printFailureMessage(SDL_GetError());
    }
    return window;
}

void Screen::printFailureMessage(const char *message) {
    std::cerr << "SDL window failure. Error: "
        << message
        << std::endl;
}

void Screen::update(PixelMatrix pixels) {
    drawBackground();
    int y = 0;
    for(auto row: pixels) {
        int x = 0;
        for(auto pixel: row) {
            if(pixel)
                drawPixel(x, y);
            ++x;
        }
        ++y;
    }

    SDL_UpdateWindowSurface(window);
}

void Screen::drawPixel(int x, int y) {
    SDL_Rect rect = SDL_Rect();
    rect.x = x*pixelWidth;
    rect.y = y*pixelHeight;
    rect.w = pixelWidth;
    rect.h = pixelHeight;
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, 0xFF, 0xFF, 0xFF));
}

void Screen::calculatePixelDimensions(int screenWidth, int screenHeight) {
    pixelWidth = screenWidth / CHIP8_DISPLAY_WIDTH;
    pixelHeight = screenHeight / CHIP8_DISPLAY_HEIGTH;
}

void Screen::drawBackground() {
    SDL_FillRect(surface,
        NULL,
        SDL_MapRGB(surface->format, 0, 0, 0));
}

Screen::~Screen() {
    SDL_DestroyWindow(window);
}
#pragma once
#include <vector>
#include <array>
#include <cstdint>

constexpr unsigned int WIDTH = 64;
constexpr unsigned int HEIGHT = 32;

typedef std::array<std::array<bool, WIDTH>, HEIGHT> PixelMatrix;

class Display {
    PixelMatrix data {};

    public:
        void clear();
        bool drawSprite(int x, int y, std::vector<uint8_t> pixels);
        PixelMatrix getData();
};
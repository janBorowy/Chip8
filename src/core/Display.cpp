#include "Display.h"


void Display::clear() {
    for(auto &row: data) {
        for(auto &pixel: row) {
            pixel = false;
        }
    }
}

bool Display::drawSprite(int x, int y, std::vector<uint8_t> pixels) {
    bool collision = false;
    for(int currentByteIndex = 0, row = y;
        currentByteIndex < pixels.size() && row < HEIGHT;
        ++row, ++currentByteIndex) {
        for(int bitPosition = 0, column = x;
            bitPosition < 8 && column < WIDTH;
            ++bitPosition, ++column
        ) {
            bool bitState = (pixels[currentByteIndex] >> (7 - bitPosition) & 0x01); 
            if(bitState) {
                if(data[row][column])
                    collision = true;
                data[row][column] = !data[row][column];
            }
        }
    }
    return collision;
}

PixelMatrix Display::getData() {
    return data;
}
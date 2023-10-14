#include <stdio.h>
#include <cstdint>
#include "Frame.h"

int main() {
    Frame *frame = new Frame;
    frame->startLoop();
    delete frame;
    return 0;
}
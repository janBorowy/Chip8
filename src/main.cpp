#include <stdio.h>
#include <cstdint>
#include <argparse/argparse.hpp>
#include <memory>
#include "Frame.h"

int main(int argc, char *argv[]) {

    argparse::ArgumentParser parser("chip8-emulator",
        "0.1",
        argparse::default_arguments::help,
        false);

    parser.add_argument("file")
        .help("path to chip8 rom file");
    parser.add_argument("-c", "--compatibility")
        .help("extensions compatibility mode");

    try {
        parser.parse_args(argc, argv);
    } catch(const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        std::cerr << parser;
        std::exit(1);
    }
    auto compatibilityMode = CHIP8_IMPLEMENTATION::ORIGINAL_CHIP8;
    if(auto compatibility = parser.present("-c")) {
        if(compatibility.value() == "schip") {
            compatibilityMode = CHIP8_IMPLEMENTATION::SCHIP;
            std::cout << "Running with SUPER-CHIP 1.0 compatibility" << std::endl;
        }
    }

    auto romFilePath = parser.get("file");
    std::unique_ptr<Frame> frame;
    try {
        frame = std::make_unique<Frame>(romFilePath, compatibilityMode);
    } catch(std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        std::exit(1);
    }
    frame->startLoop();
    return 0;
}
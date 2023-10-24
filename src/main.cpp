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

    try {
        parser.parse_args(argc, argv);
    } catch(const std::runtime_error &e) {
        std::cout << e.what() << std::endl;
        std::cerr << parser;
        std::exit(1);
    }

    auto romFilePath = parser.get("file");
    std::unique_ptr<Frame> frame;
    try {
        frame = std::make_unique<Frame>(romFilePath);
    } catch(RomFileTooLargeException &e) {
        std::cout << e.what() << std::endl;
        std::exit(1);
    }
    frame->startLoop();
    return 0;
}
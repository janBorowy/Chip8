cmake_minimum_required(VERSION 3.14)
project(chip8-emulator)

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)

file(GLOB CORE_SOURCE_FILES src/core/*.cpp)
file(GLOB UI_SOURCE_FILES src/ui/*.cpp)
file(GLOB OTHER_SOURCE_FILES src/*.cpp)
include_directories(src)
add_executable(chip8-emulator ${CORE_SOURCE_FILES} ${UI_SOURCE_FILES} ${OTHER_SOURCE_FILES})

find_package(SDL2 REQUIRED)
include_directories(
    ${SDL2_INCLUDE_DIRS})

include_directories(extern/argparse/include)

target_link_libraries(chip8-emulator ${SDL2_LIBRARIES})

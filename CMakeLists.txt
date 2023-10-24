cmake_minimum_required(VERSION 3.14)
project(chip8-emulator)

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)

file(GLOB CORE_SOURCE_FILES src/core/*.cpp)
file(GLOB OTHER_SOURCE_FILES src/*.cpp)
include_directories(src)
add_executable(chip8-emulator ${CORE_SOURCE_FILES} ${OTHER_SOURCE_FILES})

find_package(SDL2 REQUIRED)
include_directories(
    ${SDL2_INCLUDE_DIRS})

target_link_libraries(chip8-emulator
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# # For Windows: Prevent overriding the parent project's compiler/linker settings
# set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# FetchContent_MakeAvailable(googletest)

# #testing

# enable_testing()
# target_link_libraries(
#   sandbox
#   GTest::gtest_main
# )
# include(GoogleTest)
# gtest_discover_tests(sandbox)

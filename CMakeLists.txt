cmake_minimum_required(VERSION 3.14)

project(sandbox)

set(CMAKE_CXX_STANDARD_REQUIRED 17)
set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
include_directories(
    ${SDL2_INCLUDE_DIRS})

add_executable(sandbox
    src/main.cpp
    src/Screen.cpp
    src/core/Chip8.cpp
    src/core/Display.cpp
    src/Frame.cpp
    test/Chip8_test.cpp)

target_link_libraries(sandbox
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

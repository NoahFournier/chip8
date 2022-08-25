cmake_minimum_required(VERSION 3.23)
project(chip8)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

find_package(spdlog REQUIRED)

set(CMAKE_CXX_STANDARD 17)

add_executable(chip8 src/main.cpp src/Logger/Logger.cpp src/Logger/Logger.h src/Chip8/Chip8.cpp src/Chip8/Chip8.h src/Platform/Platform.cpp src/Platform/Platform.h)
target_link_libraries(chip8 PUBLIC ${SDL2_LIBRARIES}
                            PRIVATE spdlog::spdlog)

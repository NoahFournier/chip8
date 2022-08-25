#include <sstream>
#include "Chip8/Chip8.h"
#include "Logger/Logger.h"
#include "Platform/Platform.h"
#include "chrono"

const unsigned int WINDOW_WIDTH = 64;
const unsigned int WINDOW_HEIGHT = 32;
// Instructions per second
const double IPS = 700;
const double MS_PER_INSTRUCTION = 1000 / IPS;

int main(int argc, char **argv) {
    Logger::Init();

    if (argc < 2) {
        ERROR("Usage: chip8 ROM");
        exit(1);
    }

    Chip8 chip8;
    try {
        chip8.LoadROM(argv[1]);
    } catch (std::exception &e) {
        ERROR(e.what());
        exit(1);
    }
    INFO("ROM loaded!");

    Platform platform{"Chip8", WINDOW_WIDTH, WINDOW_HEIGHT,
                      sizeof(chip8.display[0]) * WINDOW_WIDTH};
    INFO("Platform initialised!");

    bool isRunning = true;
    auto lastTime = std::chrono::high_resolution_clock::now();
    INFO("Running...");
    while (isRunning) {
        // for now, ignore keyboard input
        isRunning = platform.HandleInput();

        // wait for timer
        auto now = std::chrono::high_resolution_clock::now();
        double deltaTime = std::chrono::duration<double,
                std::chrono::milliseconds::period>(now - lastTime).count();

        if (deltaTime >= MS_PER_INSTRUCTION) {
            // run Chip8
            lastTime = now;
            auto opcode = chip8.Fetch();
            chip8.Execute(opcode);
            platform.Draw(chip8.display);
        }
    }
    INFO("Quitting...");

    return 0;
}

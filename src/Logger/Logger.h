#ifndef CHIP8_LOGGER_H
#define CHIP8_LOGGER_H

#include <memory>
#include <string>
#include <spdlog/spdlog.h>

class Logger {
public:
    static void Init();
    inline static std::shared_ptr<spdlog::logger> GetLogger() { return s_Logger; }
private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

// Logger macros
#define INFO(...) Logger::GetLogger()->info(__VA_ARGS__)
#define ERROR(...) Logger::GetLogger()->error(__VA_ARGS__)
#define WARN(...) Logger::GetLogger()->warn(__VA_ARGS__)
#define DEBUG(...) Logger::GetLogger()->debug(__VA_ARGS__)


#endif //CHIP8_LOGGER_H

#include "Logger.h"

#include <spdlog/common.h>

namespace Engine {
    std::shared_ptr<spdlog::logger> Logger::coreLogger = nullptr;
    std::shared_ptr<spdlog::logger> Logger::clientLogger = nullptr;

    void Logger::Init()
    {
        // Example: [22:40:00] [Engine::Logger:Init:11] [Engine] [Thread:1000] Logger works
        spdlog::set_pattern("%^[%H:%M:%S] [%!:%#] [%n] [Thread:%t] %v%$");

        coreLogger = spdlog::stdout_color_mt("Engine");
        coreLogger->set_level(spdlog::level::trace);

        clientLogger = spdlog::stdout_color_mt("Client");
        clientLogger->set_level(spdlog::level::trace);
    }
}
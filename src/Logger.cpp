#include "Logger.h"

std::shared_ptr<spdlog::logger> Logger::clientLogger = nullptr;

void Logger::Init()
{
    clientLogger = spdlog::stdout_color_mt("Client");
    clientLogger->set_level(spdlog::level::info);
}
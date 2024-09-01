#pragma once
#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>

class Logger {
    public:
        static void Init();
        static std::shared_ptr<spdlog::logger> GetClientLogger() { return clientLogger; }
    private:
        static std::shared_ptr<spdlog::logger> clientLogger;
};

#define LOG_WITH_DETAILS(logger, level, ...) logger->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, level, __VA_ARGS__)

#if _DEBUG
    #define LOG_TRACE(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::trace, __VA_ARGS__)
    #define LOG_DEBUG(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::debug, __VA_ARGS__)
    #define LOG_INFO(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::info, __VA_ARGS__)
    #define LOG_WARN(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::warn, __VA_ARGS__)
    #define LOG_ERROR(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::err, __VA_ARGS__)
    #define LOG_CRITICAL(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::critical, __VA_ARGS__)
#elif DEBUG
    #define LOG_TRACE(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::trace, __VA_ARGS__)
    #define LOG_DEBUG(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::debug, __VA_ARGS__)
    #define LOG_INFO(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::info, __VA_ARGS__)
    #define LOG_WARN(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::warn, __VA_ARGS__)
    #define LOG_ERROR(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::err, __VA_ARGS__)
    #define LOG_CRITICAL(...) LOG_WITH_DETAILS(Logger::GetClientLogger(), spdlog::level::critical, __VA_ARGS__)
#else
    #define LOG_TRACE(...)
    #define LOG_DEBUG(...)
    #define LOG_INFO(...)
    #define LOG_WARN(...)
    #define LOG_ERROR(...)
    #define LOG_CRITICAL(...)
#endif
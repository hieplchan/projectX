#pragma once

#include <iostream>
#include <string>
#include <string_view>
#include <filesystem>
#include <format>

class Logger {
public:
    // Preformatted variant
    static inline void log_msg(std::string_view level,
        const char* file, const char* func, int line,
        std::string_view msg) {
        std::cout
            << level << ' '
            << '[' << std::filesystem::path(file).filename().string() << ':' << func << ':' << line << "] "
            << msg << '\n';
    }

    // Formatting variant
    template <typename... Args>
    static inline void log(std::string_view level,
        const char* file, const char* func, int line,
        std::string_view fmt, Args&&... args) {
        std::string out = std::vformat(fmt, std::make_format_args(args...));
        log_msg(level, file, func, line, out);
    }
};

// If you already have a formatted string, use LOG*_MSG; otherwise, use LOG* with fmt/args
#define LOG_INFO(fmt, ...)  Logger::log("[info]", __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) Logger::log("[err]", __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#define LOG_INFO_MSG(msg)   Logger::log_msg("[info]", __FILE__, __func__, __LINE__, msg)
#define LOG_ERROR_MSG(msg)  Logger::log_msg("[err]", __FILE__, __func__, __LINE__, msg)

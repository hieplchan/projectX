#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <format>

class Logger {
public:
    template<typename... Args>
    static inline void log(const std::string& cat, const std::string& fmt_str, const char* file, const char* func, int line, Args&&... args) {
        std::string formatted_message;
        if constexpr (sizeof...(args) > 0) {
            formatted_message = std::format(fmt_str, std::forward<Args>(args)...);
        }
        else {
            formatted_message = fmt_str;
        }

        std::cout << cat
            << std::format("[{}:{}] ", std::filesystem::path(file).filename().string(), func)
            << formatted_message << std::endl;
    }
};

#define LOG_INFO(fmt_str, ...) \
    Logger::log("[INFO] ", fmt_str, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(fmt_str, ...) \
    Logger::log("[ERROR] ", fmt_str, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define LOG_WARN(fmt_str, ...) \
    Logger::log("[WARN] ", fmt_str, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt_str, ...) \
    Logger::log("[CRITICAL] ", fmt_str, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

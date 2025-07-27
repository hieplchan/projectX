#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <format>

class Logger {
public:
    static inline void log(const std::string& cat, const std::string& message, const char* file, const char* func, int line) {
        std::cout << cat
                << std::filesystem::path(file).filename().string() << ":" 
                << func << ":" 
                << line << " - " 
                << message << std::endl;
    }
};

#define LOG_INFO(message) Logger::log("[INFO] ", message, __FILE__, __func__, __LINE__)
#define LOG_ERROR(message) Logger::log("[ERROR] ", message, __FILE__, __func__, __LINE__)
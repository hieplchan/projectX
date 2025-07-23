#include <iostream>
#include <format>
#include <chrono>

#include "Engine.h"
#include <Logger.h>

int main() {
    Engine engine;

    LOG_INFO(std::format("Demo app started at: {}", std::chrono::system_clock::now()));

	engine.run();    
}
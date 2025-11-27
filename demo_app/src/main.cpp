#include <iostream>
#include <format>
#include <chrono>

#include <Logger.h>
#include <Engine.h>
#include <GameObject.h>
#include <ComponentList.h>
#include <SceneLoader.h>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

int main() {
    Engine engine;

    LOG_INFO(std::format("Demo app started at: {}", std::chrono::system_clock::now()));

    if (!tryLoadSceneFromFile("scene1.json", engine)) {
        LOG_ERROR("Failed to load scene from file!");
    }

    engine.run();
}

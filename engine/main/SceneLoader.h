#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>

#include <Logger.h>
#include "Engine.h"
#include "GameObjectFactory.h"

using json = nlohmann::json;

bool tryLoadSceneFromFile(std::filesystem::path filePath, Engine& engine) {
    std::ifstream ifs(filePath.string());
    if (!ifs.is_open()) {
        LOG_ERROR(std::format("Fail to open {}", filePath));
        return false;
    }

    json jData = json::parse(ifs);

    // Load gameObjects
    if (jData["gameObjects"].is_null() || !jData["gameObject"].is_array()) {
        LOG_ERROR(std::format("{} not contain gameObjects", filePath));
        return false;
    }

    for (const auto& jGameObject : jData["gameObjects"]) {
        std::unique_ptr gameObject = createGameObjectFromJson(jGameObject);
        engine.addGameObject(std::move(gameObject));
    }

    LOG_INFO(std::format("Load {} successfully!", filePath));
    return true;
}

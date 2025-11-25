#include <filesystem>
#include <string>
#include <fstream>
#include <memory>
#include <nlohmann/json.hpp>

#include <Logger.h>
#include "Engine.h"
#include "GameObjectFactory.h"
#include "SceneLoader.h"

using json = nlohmann::json;


bool tryLoadSceneFromFile(const std::filesystem::path& filePath, Engine& engine) {
    std::ifstream ifs(filePath.string());
    if (!ifs.is_open()) {
        LOG_ERROR("Fail to open {}", filePath.string());
        return false;
    }

    json jData = json::parse(ifs);

    // Load gameObjects
    if (jData["gameObjects"].is_null() || !jData["gameObject"].is_array()) {
        LOG_ERROR("{} not contain gameObjects", filePath.string());
        LOG_ERROR("{}", jData.dump());
        return false;
    }

    for (const auto& jGameObject : jData["gameObjects"]) {
        std::unique_ptr gameObject = createGameObjectFromJson(jGameObject);
        engine.addGameObject(std::move(gameObject));
    }

    LOG_INFO("Load {} successfully!", filePath.string());
    return true;
}

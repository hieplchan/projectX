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

    // json jTest = {"key", "value"};
    // std::string out = jTest.dump();
    // LOG_INFO(std::format("JSON test output: {}", out));
    // std::vector<uint8_t> cborTest = json::to_cbor(jTest);
    // std::string cborTestStr(cborTest.begin(), cborTest.end());
    // LOG_INFO(std::format("CBOR test output: {}", cborTestStr));

    auto cameraGO = std::make_unique<GameObject>();
    auto cameraGOTF = cameraGO->addComponent<Transform>();
    auto cameraGOCam = cameraGO->addComponent<Camera>();
    cameraGO->setName("Main Camera");
    cameraGOTF->position = glm::vec3{ 0.0f, 0.0f, -10.0f };
    cameraGOCam->fovDeg = 30.0f;
    engine.addGameObject(std::move(cameraGO));

    // // Create simple game object with QuadRenderer component
    // auto quadGO = std::make_unique<GameObject>();
    // auto quadGOTF = quadGO->addComponent<Transform>();
    // quadGO->setName("Test Quad");
    // quadGO->addComponent<QuadRenderer>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
    // quadGO->addComponent<RotatorComp>();
    // quadGOTF->rotation.y = 45.0f;

    // engine.addGameObject(std::move(quadGO));

    if (!tryLoadSceneFromFile("scene1.json", engine)) {
        LOG_ERROR("Failed to load scene from file!");
    }

    engine.run();
}

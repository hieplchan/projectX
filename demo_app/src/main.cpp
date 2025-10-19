#include <iostream>
#include <format>
#include <chrono>

#include <Logger.h>
#include <Engine.h>
#include <GameObject.h>
#include <Transform.h>
#include <QuadRenderer.h>
#include <Camera.h>

#include <components/RotatorComp.h>

int main() {
    Engine engine;

    LOG_INFO(std::format("Demo app started at: {}", std::chrono::system_clock::now()));

    auto cameraGO = std::make_unique<GameObject>();
    auto cameraGOTF = cameraGO->addComponent<Transform>();
    auto cameraGOCam = cameraGO->addComponent<Camera>();
    cameraGO->setName("Main Camera");
    cameraGOTF->position = glm::vec3{ 0.0f, 0.0f, -10.0f };
    cameraGOCam->fovDeg = 30.0f;

    // Create simple game object with QuadRenderer component
    auto quadGO = std::make_unique<GameObject>();
    auto quadGOTF = quadGO->addComponent<Transform>();
    quadGO->setName("Test Quad");
    quadGO->addComponent<QuadRenderer>(glm::vec4{ 1.0f, 0.0f, 0.0f, 1.0f });
    quadGO->addComponent<RotatorComp>();
    quadGOTF->rotation.y = 45.0f;

    engine.addGameObject(std::move(cameraGO));
    engine.addGameObject(std::move(quadGO));

    engine.run();
}

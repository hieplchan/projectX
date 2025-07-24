#include <iostream>
#include <format>
#include <chrono>

#include <Logger.h>
#include <Engine.h>
#include <GameObject.h>
#include <components/Transform.h>
#include <components/QuadRenderer.h>

int main() {
    Engine engine;

    LOG_INFO(std::format("Demo app started at: {}", std::chrono::system_clock::now()));

    // Create simple game object with QuadRenderer component
    auto go = std::make_unique<GameObject>();
    
    // auto tfComp = go->addComponent<Transform>();
    // tfComp->position = glm::vec3(50.0f, 100.0f, 0.0f);
    // tfComp->scale    = glm::vec3(100.0f, 50.0f, 1.0f);
    
    // go->addComponent<QuadRenderer>(glm::vec4{1.0f, 0.0f, 0.0f, 1.0f});

    engine.addGameObject(std::move(go));

	engine.run();    
}
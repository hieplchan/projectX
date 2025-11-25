#pragma once

#include <memory>
#include <nlohmann/json.hpp>

#include <Logger.h>
#include "GameObject.h"
#include "GameObjectFactory.h"
#include "ComponentFactory.h"

using json = nlohmann::json;

std::unique_ptr<GameObject> createGameObjectFromJson(const json& jData) {
    auto gameObject = std::make_unique<GameObject>();

    for (const auto& jComp : jData["components"]) {
        std::string_view type = jComp["type"];
        std::unique_ptr<Component> component = createComponentFromJson(type, jComp);
        if (component) {
            gameObject->addComponent(std::move(component));
        } else {
            LOG_ERROR(std::format("GameObjectFactory: Failed to create component of type {}", type));
        }
    }

    return gameObject;
}

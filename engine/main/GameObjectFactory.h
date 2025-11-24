#pragma once

#include <memory>
#include <nlohmann/json.hpp>

#include "GameObject.h"

using json = nlohmann::json;

std::unique_ptr<GameObject> createGameObjectFromJson(const json& jData);

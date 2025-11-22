#pragma once

#include <memory>
#include <nlohmann/json.hpp>

#include "ComponentBase.h"
#include "ComponentTypes.h"

std::unique_ptr<Component> createCompFromJson(std::string_view typeName, const nlohmann::json& jData);

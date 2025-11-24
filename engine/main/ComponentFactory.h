#pragma once

#include <memory>
#include <functional>
#include <string_view>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "ComponentBase.h"
#include "ComponentList.h"

using json = nlohmann::json;
using ComponentFactoryFunc = std::function<std::unique_ptr<Component>(const json&)>;
using ComponentFactoryFuncMap = std::unordered_map<std::string_view, ComponentFactoryFunc>;

std::unique_ptr<Component> createComponentFromJson(
    std::string_view typeName,
    const json& jData
);

template<ComponentType T>
void populateComponentFromJson(
    T* comp,
    const json& jData
);

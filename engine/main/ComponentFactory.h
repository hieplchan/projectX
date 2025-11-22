#pragma once

#include <memory>
#include <functional>
#include <string_view>
#include <unordered_map>
#include <nlohmann/json.hpp>

#include "ComponentBase.h"
#include "ComponentList.h"

using json = nlohmann::json;
using ComponentPtr = std::unique_ptr<Component>;
using ComponentFactoryFunc = std::function<ComponentPtr(const json&)>;
using ComponentFactoryFuncMap = std::unordered_map<std::string_view, ComponentFactoryFunc>;

ComponentPtr createCompFromJson(
    std::string_view typeName,
    const json& jData
);

template<ComponentType T>
void populateCompFromJson(
    T* comp,
    const json& jData
);

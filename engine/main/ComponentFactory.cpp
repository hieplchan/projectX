#include <algorithm>
#include <ranges>

#include <Logger.h>
#include <ComponentFactory.h>
#include <Metadata.h>

namespace {
#pragma region JSON Parsing Helpers
template<ComponentType T>
void parseBoolFields(T* comp, const json& jData, const Property<T>& typeInfo) {
    for (const BoolField<T>& f : typeInfo.bools) {
        const std::string_view& key = f.label;
        if (!jData.contains(key)) {
            LOG_WARN("jData not contain {}", key);
            continue;
        }
        if (!jData[key].is_boolean()) {
            LOG_WARN("jData {} not bool", key);
            continue;
        }
        comp->*(f.member) = jData[key].get<bool>();
    }
}

// json enum can be String or Int
template<ComponentType T>
void parseEnumFields(T* comp, const json& jData, const Property<T>& typeInfo) {
    for (const EnumField<T>& f : typeInfo.enums) {
        const std::string_view& key = f.label;
        if (!jData.contains(key)) {
            LOG_WARN("jData not contain {}", key);
            continue;
        }

        if (jData[key].is_string()) { // json enum can be String
            std::string enumStr = jData[key].get<std::string>();
            auto it = std::ranges::find(f.names, enumStr);
            if (it == f.names.end()) {
                LOG_WARN("jData {} enum string {} not found", key, enumStr);
                continue;
            }
            comp->*(f.member) = f.values[std::distance(f.names.begin(), it)];
        } else if (jData[key].is_number_integer()) { // json enum can also be Int
            int enumInt = jData[key].get<int>();
            if (auto it = std::ranges::find(f.values, enumInt); it == f.values.end()) {
                LOG_WARN("jData {} enum int {} not found", key, enumInt);
                continue;
            }
            comp->*(f.member) = enumInt;
        } else {
            LOG_WARN("jData {} not enum or int", key);
        }
    }
}

template<ComponentType T>
void parseNumericIntFields(T* comp, const json& jData, const Property<T>& typeInfo) {
    for (const NumericField<T, int>& f : typeInfo.ints) {
        const std::string_view& key = f.label;
        if (!jData.contains(key)) {
            LOG_WARN("jData not contain {}", key);
            continue;
        }
        if (!jData[key].is_number_integer()) {
            LOG_WARN("jData {} not int", key);
            continue;
        }
        comp->*(f.member) = jData[key].get<int>();
    }
}

template<ComponentType T>
void parseNumericFloatFields(T* comp, const json& jData, const Property<T>& typeInfo) {
    for (const NumericField<T, float>& f : typeInfo.floats) {
        const std::string_view& key = f.label;
        if (!jData.contains(key)) {
            LOG_WARN("jData not contain {}", key);
            continue;
        }
        if (!jData[key].is_number()) {
            LOG_WARN("jData {} not number", key);
            continue;
        }
        comp->*(f.member) = jData[key].get<float>();
    }
}

template<ComponentType T>
void parseStringFields(T* comp, const json& jData, const Property<T>& typeInfo) {
    for (const StringField<T>& f : typeInfo.strings) {
        const std::string_view& key = f.label;
        if (!jData.contains(key)) {
            LOG_WARN("jData not contain {}", key);
            continue;
        }
        if (!jData[key].is_string()) {
            LOG_WARN("jData {} not string", key);
            continue;
        }
        comp->*(f.member) = jData[key].get<std::string>();
    }
}

template<ComponentType T>
void parseVec3Fields(T* comp, const json& jData, const Property<T>& typeInfo) {
for (const Vec3Field<T>& f : typeInfo.vec3s) {
        const std::string_view& key = f.label;
        if (!jData.contains(key)) {
            LOG_WARN("jData not contain {}", key);
            continue;
        }
        if (!jData[key].is_array() || jData[key].size() != 3) {
            LOG_WARN("jData {} not vec3", key);
            continue;
        }
        comp->*(f.vec3) = glm::vec3 {
            jData[key][0].get<float>(),
            jData[key][1].get<float>(),
            jData[key][2].get<float>()
        };
    }
}

template<ComponentType T>
void parseColorFields(T* comp, const json& jData, const Property<T>& typeInfo) {
    for (const ColorField<T>& f : typeInfo.colors) {
        const std::string_view& key = f.label;
        if (!jData.contains(key)) {
            LOG_WARN("jData not contain {}", key);
            continue;
        }
        if (!jData[key].is_array() || jData[key].size() != 4) {
            LOG_WARN("jData {} not vec4", key);
            continue;
        }
        comp->*(f.color) = glm::vec4 {
            jData[key][0].get<float>(),
            jData[key][1].get<float>(),
            jData[key][2].get<float>(),
            jData[key][3].get<float>()
        };
    }
}
#pragma endregion

template<ComponentType T>
void populateComponentFromJson(T* comp, const json& jData) {
    const Property<T>& typeInfo = reflect<T>();

    parseBoolFields<T>(comp, jData, typeInfo);
    parseEnumFields<T>(comp, jData, typeInfo);
    parseNumericIntFields<T>(comp, jData, typeInfo);
    parseNumericFloatFields<T>(comp, jData, typeInfo);
    parseStringFields<T>(comp, jData, typeInfo);
    parseVec3Fields<T>(comp, jData, typeInfo);
    parseColorFields<T>(comp, jData, typeInfo);
}

ComponentFactoryFuncMap buildFactoryFuncMap() {
    ComponentFactoryFuncMap factoryMap;

    #define REGISTER_COMPONENT_TYPE(Type) \
    factoryMap.try_emplace(#Type, [](const json& jData) -> std::unique_ptr<Component> { \
        auto obj = std::make_unique<Type>(); \
        populateComponentFromJson<Type>(obj.get(), jData); \
        return obj; \
    });

    COMPONENT_LIST(REGISTER_COMPONENT_TYPE)
    #undef REGISTER_COMPONENT_TYPE

    return factoryMap;
}
} // anonymous namespace

std::unique_ptr<Component> createComponentFromJson(std::string_view typeName, const json& jData) {
    static const ComponentFactoryFuncMap factoryMap = buildFactoryFuncMap();
    if (auto it = factoryMap.find(typeName); it != factoryMap.end()) {
        return it->second(jData);
    }

    LOG_ERROR(std::format("ComponentFactory: Unknown component type {}", typeName));
    return nullptr;
}

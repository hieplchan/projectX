#include <Logger.h>
#include <ComponentFactory.h>
#include <Metadata.h>

namespace {
template<ComponentType T>
void populateComponentFromJson(T* comp, const json& jData) {
    const Property<T>& typeInfo = reflect<T>();
    // LOG_INFO("Populating component of type {}", typeInfo.name);

    for (const BoolField<T>& f : typeInfo.bools) {
        // LOG_INFO("Parse BoolField {}", f.label);
    }

    for (const EnumField<T>& f : typeInfo.enums) {
        // LOG_INFO("Parse EnumField {}", f.label);
    }

    for (const NumericField<T, int>& f : typeInfo.ints) {
        // LOG_INFO("Parse NumericField<int> {}", f.label);
    }

    for (const NumericField<T, float>& f : typeInfo.floats) {
        // LOG_INFO("Parse NumericField<int> {}", f.label);
    }

    for (const Vec3Field<T>& f : typeInfo.vec3s) {
        std::string key = std::string(f.label);
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

    for (const ColorField<T>& f : typeInfo.colors) {
        // LOG_INFO("Parse ColorField {}", f.label);
    }
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

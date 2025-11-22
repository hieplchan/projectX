#include <Logger.h>
#include <ComponentFactory.h>

namespace {
ComponentFactoryFuncMap buildFactoryFuncMap() {
    ComponentFactoryFuncMap factoryMap;

    #define REGISTER_COMPONENT_TYPE(Type) \
    factoryMap.try_emplace(#Type, [](const json& jData) -> ComponentPtr { \
        auto obj = std::make_unique<Type>(); \
        populateCompFromJson<Type>(obj.get(), jData); \
        return obj; \
    });

    COMPONENT_LIST(REGISTER_COMPONENT_TYPE)
    #undef REGISTER_COMPONENT_TYPE

    return factoryMap;
}
} // anonymous namespace

ComponentPtr createCompFromJson(std::string_view typeName, const json& jData) {
    static const ComponentFactoryFuncMap factoryMap = buildFactoryFuncMap();
    if (auto it = factoryMap.find(typeName); it != factoryMap.end()) {
        return it->second(jData);
    }

    LOG_ERROR("ComponentFactory: Unknown component type {}", typeName);
    return nullptr;
}

template<ComponentType T>
void populateCompFromJson(T* comp, const json& jData) {

}

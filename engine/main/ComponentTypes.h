#include <unordered_map>

#include <Metadata.h>

#include <components/Transform.h>
#include <components/Camera.h>
#include <components/QuadRenderer.h>
#include <components/RotatorComp.h>

Property getProperty(std::string_view componentName) {
    static const std::unordered_map<std::string_view, Property> propertyMap = {
        { "Transform", reflect<Transform>() },
        { "Camera", reflect<Camera>() },
        { "QuadRenderer", reflect<QuadRenderer>() },
        { "RotatorComp", reflect<RotatorComp>() }
    };

    auto it = propertyMap.find(componentName);
    if (it != propertyMap.end()) {
        return it->second;
    }

    return Property{}; // Return an empty Property if not found
}

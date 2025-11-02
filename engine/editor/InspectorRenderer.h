#pragma once

#ifdef ENABLE_IMGUI
#include <Metadata.h>
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Inspector {

template <typename Object>
void drawFromProperty(Object* obj, const Property<Object>& prop) {
    ImGui::PushID(obj);

    ImGui::SeparatorText(prop.name.data());

    for (const auto& field : prop.bools) {
        ImGui::Checkbox(
            field.label.data(),
            &(obj->*(field.member))
        );
    }

    for (const auto& field : prop.enums) {
        int currVal = (obj->*(field.member));

        int currIdx = 0;
        for (std::size_t i = 0; i < field.values.size(); i++) {
            if (field.values[i] == currVal) {
                currIdx = static_cast<int>(i);
                break;
            }
        }

        std::vector<const char*> items;
        items.reserve(field.names.size());
        for (auto& name : field.names) {
            items.push_back(name.data());
        }

        if (ImGui::Combo(field.label.data(), &currIdx, items.data(), static_cast<int>(items.size()))) {
            (obj->*(field.member)) = field.values[currIdx];
        }
    }

    for (const auto& field : prop.floats) {
        ImGui::DragFloat(
            field.label.data(),
            &(obj->*(field.member)),
            field.step,
            field.min,
            field.max
        );
    }

    for (const auto& field : prop.ints) {
        ImGui::DragInt(
            field.label.data(),
            &(obj->*(field.member)),
            field.step,
            field.min,
            field.max
        );
    }

    for (const auto& field : prop.vec3s) {
        ImGui::DragFloat3(
            field.label.data(),
            glm::value_ptr((obj->*(field.vec3))),
            field.step,
            field.min,
            field.max
        );
    }

    for (const auto& field : prop.colors) {
        ImGui::ColorEdit4(
            field.label.data(),
            glm::value_ptr((obj->*(field.color)))
        );
    }

    ImGui::PopID();
}
} // namespace Inspector

#endif

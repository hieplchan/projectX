#pragma once

#ifdef ENABLE_IMGUI
#include <Metadata.h>
#include <imgui.h>

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

    for (const auto& field : prop.floats) {
        ImGui::SliderFloat(
            field.label.data(),
            &(obj->*(field.member)),
            field.min,
            field.max
        );
    }

    for (const auto& field : prop.ints) {
        ImGui::SliderInt(
            field.label.data(),
            &(obj->*(field.member)),
            field.min,
            field.max
        );
    }

    ImGui::PopID();
}
} // namespace Inspector

#endif

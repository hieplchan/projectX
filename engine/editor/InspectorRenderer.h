#pragma once

#ifdef ENABLE_IMGUI
#include <InspectorMetadata.h>
#include <imgui.h>

namespace Inspector {

template <typename Object>
void drawFromProperty(Object* instance, const Property<Object>& prop) {
    ImGui::PushID(instance);

    ImGui::SeparatorText(prop.name.data());

    for (const auto& field : prop.bools) {
        ImGui::Checkbox(field.label.data(), &(instance->*(field.member)));
    }

    ImGui::PopID();
}
} // namespace Inspector

#endif

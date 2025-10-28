#ifdef ENABLE_IMGUI
#include "InspectorRenderer.h"

namespace Inspector{
template <typename Object>
void drawFromProperty(const Object& instance, const Property<Object>& prop) {
    ImGui::PushID(instance);

    ImGui::SeparatorText(prop.name.data());

    for (const BoolField& field : prop.bools) {
        ImGui::Checkbox(field.label.data(), &(instance.*(field.member)));
    }

    ImGui::PopID();
}
} // namespace Inspector

#endif

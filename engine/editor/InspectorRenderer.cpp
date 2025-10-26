#ifdef ENABLE_IMGUI
#include <imgui.h>

#include "InspectorRenderer.h"

namespace Inspector{
template <typename Object>
void drawFromProperty(const Object& instance, const Property<Object>& prop) {
    ImGui::SeparatorText(prop.name.data());

    for (const auto& field : prop.bools) {
        bool& v = instance.*(field.member);
        ImGui::Checkbox(field.label.data(), &v);
    }
}
} // namespace Inspector

#endif

#ifdef ENABLE_IMGUI
#include <imgui.h>

#include "InspectorMetadata.h"
#include "Inspector.h"

namespace Inspector {

template<typename T>
void drawInspector(T* instance) {
    ImGui::PushID(instance);

    const auto& prop = reflect<T>();
    ImGui::SeparatorText(prop.name.data());

    for (const auto& field : prop.bools) {
        bool& v = instance.*(field.member);
        ImGui::Checkbox(field.label.data(), &v);
    }

    ImGui::PopID();
}

} // namepsace Inspector
#endif

#ifdef ENABLE_IMGUI
#include "InspectorRenderer.h"

namespace Inspector{
namespace {
template <typename Object>
void drawFromProperty(const Object& obj, const Property<Object>& prop) {

}
} // namespace anonymous

template <typename Object>
void drawInspector(Object& obj) {
    const Property<Object>& prop = reflect<Object>();
    ImGui::PushID(obj);
    drawFromProperty(obj, prop);
    ImGui::PopID();
}

} // namespace Inspector

#endif

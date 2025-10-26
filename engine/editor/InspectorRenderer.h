#pragma once

#ifdef ENABLE_IMGUI
#include <Inspector.h>
#include <Component.h>
#include <imgui.h>

namespace Inspector {

template<typename Object>
void drawFromProperty(const Object& instance, const Property<Object>& prop);

} // namespace Inspector

#endif

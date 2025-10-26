#pragma once

#ifdef ENABLE_IMGUI
#include <Inspector.h>

namespace Inspector {

template<typename Object>
void drawFromProperty(Object& instance, const Property<Object>& prop);
} // namespace Inspector

#endif

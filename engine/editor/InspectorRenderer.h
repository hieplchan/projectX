#pragma once

#ifdef ENABLE_IMGUI
#include <Inspector.h>
#include <imgui.h>

namespace Inspector {

template<typename Object>
void drawInspector(Object& obj);

} // namespace Inspector

#endif

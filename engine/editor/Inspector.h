#pragma once

#ifdef ENABLE_IMGUI

class Component;

namespace Inspector {

template <typename T>
void drawInspector(T* instance);

} // namespace Inspector
#endif

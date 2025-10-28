#ifdef ENABLE_IMGUI
#include <Component.h>

#include "Inspector.h"

template<typename T>
void ComponentBase<T>::onDrawInspector() {
    Inspector::drawInspector<T>();
}

#endif

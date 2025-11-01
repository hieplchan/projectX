#include "RotatorComp.h"

#ifdef ENABLE_IMGUI
#include <InspectorRenderer.h>

void RotatorComp::onInspectorGUI() {
    Inspector::drawFromProperty<RotatorComp>(this, reflect<RotatorComp>());
}

#endif

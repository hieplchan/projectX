#include "Transform.h"
#include "SpriteRenderer.h"

#ifdef ENABLE_IMGUI
#include <InspectorRenderer.h>

void SpriteRenderer::onInspectorGUI() {
    Inspector::drawFromProperty<SpriteRenderer>(this, reflect<SpriteRenderer>());
}
#endif

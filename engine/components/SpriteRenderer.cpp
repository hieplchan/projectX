#include <bgfx/bgfx.h>

#include <bgfx_utils.h>
#include "Logger.h"
#include "Transform.h"
#include "SpriteRenderer.h"

#ifdef ENABLE_IMGUI
#include <InspectorRenderer.h>

void SpriteRenderer::onInspectorGUI() {
    Inspector::drawFromProperty<SpriteRenderer>(this, reflect<SpriteRenderer>());
}
#endif

void SpriteRenderer::onDeserialized() {
    LOG_INFO("SpriteRenderer deserialized with texturePath: {}", texturePath);

    bgfx::TextureHandle tex = loadTexture(texturePath);
    if (!bgfx::isValid(tex)) {
        LOG_ERROR("SpriteRenderer: Failed to load texture {}", texturePath);
    } else {
        LOG_INFO("SpriteRenderer: Successfully loaded texture {}", texturePath);
    }
}

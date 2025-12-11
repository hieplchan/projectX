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

SpriteRenderer::SpriteRenderer() {
    bgfx::ShaderHandle vsh = loadShader(kVertexShaderName);
    bgfx::ShaderHandle fsh = loadShader(kFragmentShaderName);
    progHandle = bgfx::createProgram(vsh, fsh, true);
    if (!bgfx::isValid(progHandle)) {
        LOG_ERROR("SpriteRenderer: Failed to create shader program");
    }
}

SpriteRenderer::~SpriteRenderer() {
    BGFX_SAFE_DESTROY_HANDLE(texHandle);
    BGFX_SAFE_DESTROY_HANDLE(progHandle);
}

void SpriteRenderer::onDeserialized() {
    bgfx::TextureHandle tex = loadTexture(texFilePath);
    if (!bgfx::isValid(tex)) {
        LOG_ERROR("SpriteRenderer: Failed to load texture {}", texFilePath);
        return;
    }
    texHandle = tex;
}

#include <bgfx_utils.h>
#include "Logger.h"
#include "Transform.h"
#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer() {
    bgfx::ShaderHandle vsh = loadShader(kVertexShaderName);
    bgfx::ShaderHandle fsh = loadShader(kFragmentShaderName);
    m_hProg = bgfx::createProgram(vsh, fsh, true);
    if (!bgfx::isValid(m_hProg)) {
        LOG_ERROR("SpriteRenderer: Failed to create shader program");
    }

    m_uTint = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
    m_uTexSampler = bgfx::createUniform("s_tex0", bgfx::UniformType::Sampler);
}

SpriteRenderer::~SpriteRenderer() {
    BGFX_SAFE_DESTROY_HANDLE(m_uTint);
    BGFX_SAFE_DESTROY_HANDLE(m_uTexSampler);
    BGFX_SAFE_DESTROY_HANDLE(m_hTex);
    BGFX_SAFE_DESTROY_HANDLE(m_hProg);
}

void SpriteRenderer::onDeserialized() {
    bgfx::TextureHandle tex = loadTexture(texFilePath);
    if (!bgfx::isValid(tex)) {
        LOG_ERROR("SpriteRenderer: Failed to load texture {}", texFilePath);
        return;
    }
    m_hTex = tex;
}

#pragma once

#include <string>
#include <vector>
#include <format>
#include <filesystem>

#include <bgfx/bgfx.h>
#include <bimg/decode.h>
#include <bx/allocator.h>

#include <common/constants.h>
#include <utils/file_utils.h>
#include <Logger.h>

using OptionalByteBuffer = std::optional<std::vector<uint8_t>>;

#define BGFX_SAFE_DESTROY_HANDLE(handle) \
    do { \
        if (bgfx::isValid(handle)) { \
            bgfx::destroy(handle); \
        } \
    } while (0)

inline bgfx::ShaderHandle loadShader(const std::string& name) {
    const std::filesystem::path path = std::filesystem::path(shaderDir) / (name + ".bin");
    OptionalByteBuffer data = tryLoadFileWithSDL(path);
    if (!data.has_value()) {
        LOG_ERROR("Failed to read shader {}", path.string());
        return BGFX_INVALID_HANDLE;
    }

    const bgfx::Memory* mem = bgfx::copy(data->data(), static_cast<uint32_t>(data->size()));
    bgfx::ShaderHandle sh = bgfx::createShader(mem);
    if (!bgfx::isValid(sh)) {
        LOG_ERROR("Failed to create shader {}", path.string());
    }
    return sh;
}

inline bgfx::ProgramHandle loadProgram(const std::string& vsName, const std::string& fsName) {
    bgfx::ShaderHandle vsh = loadShader(vsName);
    bgfx::ShaderHandle fsh = loadShader(fsName);
    if (!bgfx::isValid(vsh) || !bgfx::isValid(fsh)) {
        BGFX_SAFE_DESTROY_HANDLE(vsh);
        BGFX_SAFE_DESTROY_HANDLE(fsh);
        return BGFX_INVALID_HANDLE;
    }

    bgfx::ProgramHandle program = bgfx::createProgram(vsh, fsh, true);
    if (!bgfx::isValid(program)) {
        LOG_ERROR("Failed to create program from {} and {}", vsName, fsName);
        BGFX_SAFE_DESTROY_HANDLE(vsh);
        BGFX_SAFE_DESTROY_HANDLE(fsh);
        return BGFX_INVALID_HANDLE;
    }
    return program;
}

inline bgfx::TextureHandle loadTexture(std::filesystem::path path) {
    static bx::DefaultAllocator g_allocator;

    // load file
    OptionalByteBuffer data = tryLoadFileWithSDL(path);
    if (!data.has_value()) {
        LOG_ERROR("Failed to read texture {}", path.string());
        return BGFX_INVALID_HANDLE;
    }

    // auto detect format & decode image
    bimg::ImageContainer* imgContainer = bimg::imageParse(
        &g_allocator,
        data->data(),
        static_cast<uint32_t>(data->size())
    );
}

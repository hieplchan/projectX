#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <format>
#include <filesystem>

#include <bgfx/bgfx.h>
#include <bx/error.h>
#include <bimg/decode.h>
#include <bx/allocator.h>
#include <magic_enum/magic_enum.hpp>

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

inline bgfx::ShaderHandle loadShader(std::string_view name) {
    const std::filesystem::path path = std::filesystem::path(kShaderDir) / name;
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

inline bgfx::ProgramHandle loadProgram(std::string_view vsName, std::string_view fsName) {
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

inline bgfx::TextureHandle loadTexture(const std::string& texturePath) {
    auto path = std::filesystem::path(kTextureDir) / texturePath;
    static bx::DefaultAllocator g_allocator;

    // load file
    OptionalByteBuffer data = tryLoadFileWithSDL(path);
    if (!data.has_value()) {
        LOG_ERROR("Failed to read texture {}", path.string());
        return BGFX_INVALID_HANDLE;
    }

    // auto detect format & decode image
    bx::Error err;
    bimg::ImageContainer* imgContainer = bimg::imageParse(
        &g_allocator,
        data->data(),
        static_cast<uint32_t>(data->size()),
        bimg::TextureFormat::Count,
        &err
    );
    if (!imgContainer) {
        LOG_ERROR("Failed to parse image {}, err: {}", path.string(), err.getMessage().getCPtr());
        return BGFX_INVALID_HANDLE;
    }

    LOG_INFO("Loaded image {} ({}x{}, mips: {}, layers: {}, format: {})",
        path.string(),
        imgContainer->m_width,
        imgContainer->m_height,
        static_cast<int>(imgContainer->m_numMips),
        static_cast<int>(imgContainer->m_numLayers),
        magic_enum::enum_name(imgContainer->m_format)
    );

    // create bgfx texture
    const bgfx::Memory* mem = bgfx::makeRef(imgContainer->m_data, imgContainer->m_size);
    bgfx::TextureHandle tex = bgfx::createTexture2D(
        static_cast<uint16_t>(imgContainer->m_width),
        static_cast<uint16_t>(imgContainer->m_height),
        imgContainer->m_numMips > 1,
        imgContainer->m_numLayers,
        static_cast<bgfx::TextureFormat::Enum>(imgContainer->m_format),
        0, // default no flags
        mem
    );

    // cleanup
    bimg::imageFree(imgContainer);
    if (!bgfx::isValid(tex)) {
        LOG_ERROR("Failed to create texture {}", path.string());
        return BGFX_INVALID_HANDLE;
    }

    return tex;
}

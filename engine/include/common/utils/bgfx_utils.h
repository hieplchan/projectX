#pragma once

#include <bgfx/bgfx.h>
#include <string>
#include <vector>
#include <format>

#include <common/constants.h>
#include <common/utils/file_utils.h>
#include "../../Logger.h"

#define BGFX_SAFE_DESTROY_HANDLE(handle) \
    do { \
        if (bgfx::isValid(handle)) { \
            bgfx::destroy(handle); \
        } \
    } while (0)

inline bgfx::ShaderHandle loadShader(const std::string& name) {
    const std::filesystem::path path = std::filesystem::path(shaderDir) / (name + ".bin");
    auto data = tryLoadFileWithSDL(path);
    if (!data) {
        LOG_ERROR("Failed to read shader {}", path.string());
        return BGFX_INVALID_HANDLE;
    }

    const bgfx::Memory* mem = bgfx::copy(data->data(), (uint32_t)data->size());
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

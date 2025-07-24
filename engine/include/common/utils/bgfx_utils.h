#pragma once

#include <bgfx/bgfx.h>

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
    std::filesystem::path shaderPath = shaderDir + name + ".bin";
    
    auto data = tryLoadFileWithSDL(shaderPath);
    if (!data) {
        LOG_ERROR(std::format("Failed to load shader: {}", shaderPath.string()));
        return BGFX_INVALID_HANDLE;
    }

    return bgfx::createShader(bgfx::makeRef(data->data(), data->size()));
}

inline bgfx::ProgramHandle loadProgram(const std::string& vsName, const std::string& fsName) {
    bgfx::ShaderHandle vertexShader = loadShader(vsName);
    if (!bgfx::isValid(vertexShader)) {
        LOG_ERROR(std::format("Vertex shader {} is invalid", vsName));
        return BGFX_INVALID_HANDLE;
    }

    bgfx::ShaderHandle fragmentShader = loadShader(fsName);
    if (!bgfx::isValid(fragmentShader)) {
        LOG_ERROR(std::format("Fragment shader {} is invalid", fsName));
        bgfx::destroy(vertexShader);
        return BGFX_INVALID_HANDLE;
    }

    bgfx::ProgramHandle program = bgfx::createProgram(vertexShader, fragmentShader, true);
    if (!bgfx::isValid(program)) {
        LOG_ERROR(std::format("Failed to create program from {} and {}", vsName, fsName));
        BGFX_SAFE_DESTROY_HANDLE(vertexShader);
        BGFX_SAFE_DESTROY_HANDLE(fragmentShader);
        return BGFX_INVALID_HANDLE;
    }

    return program;
}
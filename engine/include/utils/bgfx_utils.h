#pragma once

#include <bgfx/bgfx.h>

#define BGFX_SAFE_DESTROY_HANDLE(handle) \
    do { \
        if (bgfx::isValid(handle)) { \
            bgfx::destroy(handle); \
        } \
    } while (0)
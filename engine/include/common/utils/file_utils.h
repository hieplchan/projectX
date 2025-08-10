#pragma once

#include <filesystem>
#include <optional>

#include <SDL.h>
#include "../../Logger.h"

/**
 * @brief Loads a file into a byte buffer using SDL_RWops.
 * @param path Path to the file.
 * @return File contents as a byte buffer, or std::nullopt on failure.
 */
inline std::optional<std::vector<uint8_t>> tryLoadFileWithSDL(const std::filesystem::path& path) {
    SDL_RWops* rw = SDL_RWFromFile(path.string().c_str(), "rb");
    if (!rw) {
        LOG_ERROR(std::format("Cannot open file: {}", path.string()));
        return std::nullopt;
    }

    Sint64 size = SDL_RWsize(rw);
    if (size <= 0) {
        LOG_ERROR(std::format("Invalid file size {}", path.string()));
        SDL_RWclose(rw);
        return std::nullopt;
    }

    std::vector<uint8_t> buf(static_cast<size_t>(size));
    size_t readSize = SDL_RWread(rw, buf.data(), 1, buf.size());
    if (readSize != buf.size()) {
        LOG_ERROR(std::format("Read error ({} of {} bytes): {}", readSize, buf.size(), path.string()));
        SDL_RWclose(rw);
        return std::nullopt;
    }

    SDL_RWclose(rw);
    return buf;
}

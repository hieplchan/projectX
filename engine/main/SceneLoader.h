#pragma once

#include "engine_export.h"

#include <filesystem>

#include "Engine.h"

[[nodiscard]] bool ENGINE_EXPORT tryLoadSceneFromFile(const std::filesystem::path& filePath, Engine& engine);

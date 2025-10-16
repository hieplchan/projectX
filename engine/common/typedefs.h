#pragma once

#include <string>

struct WindowSettings {
    uint16_t width = 800;
    uint16_t height = 600;
    std::string title = "Demo App";

    struct ViewIds {
        uint16_t world = 1;
        uint16_t ui = 11;
    } viewIds;

    int msaaSamples = 1;
    bool bUsingVSync = true;
};

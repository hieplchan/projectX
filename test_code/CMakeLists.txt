cmake_minimum_required(VERSION 3.15)

if (WIN32)
    add_subdirectory(bgfx_quad)
    add_subdirectory(test_nlohman_json)
    add_subdirectory(test_x_macro)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ENGINE_SOURCES CMAKE_CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/main/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/components/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/editor/*.cpp
)

add_library(engine SHARED ${ENGINE_SOURCES})

set_target_properties(engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/main
    ${CMAKE_CURRENT_SOURCE_DIR}/components
    ${CMAKE_CURRENT_SOURCE_DIR}/editor
    ${CMAKE_CURRENT_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}/utils

    ${CMAKE_CURRENT_BINARY_DIR} # for engine_export.h

    ${CMAKE_SOURCE_DIR}/thirdparty/glm
    ${CMAKE_SOURCE_DIR}/thirdparty/SDL/include
    ${CMAKE_SOURCE_DIR}/thirdparty/bgfx.cmake/bgfx/include
    ${CMAKE_SOURCE_DIR}/thirdparty/bgfx.cmake/bx/include
    ${CMAKE_SOURCE_DIR}/thirdparty/magic_enum/include
)

target_link_libraries(engine PRIVATE
    SDL2
    bx bimg bimg_decode bgfx
    imgui bgfx-imgui-backend
    nlohmann_json::nlohmann_json
)

target_compile_definitions(engine PUBLIC
    $<$<CONFIG:Debug>:ENABLE_IMGUI>
)

# Generate "engine_export.h" for DLL export
include(GenerateExportHeader)
generate_export_header(engine)

# Copy DLLs to output directory
add_custom_command(TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2>
        $<TARGET_FILE_DIR:engine>
)

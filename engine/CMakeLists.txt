add_library(engine SHARED
    src/engine.cpp
)

set_target_properties(engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR} # for engine_export.h

    ${CMAKE_SOURCE_DIR}/thirdparty/SDL/include
    ${CMAKE_SOURCE_DIR}/thirdparty/bgfx.cmake/bgfx/include
)

target_link_libraries(engine PRIVATE SDL2 bgfx)

# Generate "engine_export.h" for DLL export
include(GenerateExportHeader)
generate_export_header(engine)

# Copy DLLs to output directory
add_custom_command(TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2>
        $<TARGET_FILE_DIR:engine>
)
add_library(engine SHARED
    src/engine.cpp
)

target_include_directories(engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

# Generate "engine_export.h" for DLL export
include(GenerateExportHeader)
generate_export_header(engine)
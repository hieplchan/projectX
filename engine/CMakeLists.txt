set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE ENGINE_SOURCES CMAKE_CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/components/*.cpp
)

add_library(engine SHARED ${ENGINE_SOURCES})

set_target_properties(engine PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(engine PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  ${CMAKE_CURRENT_SOURCE_DIR}/components
  ${CMAKE_CURRENT_BINARY_DIR} # for engine_export.h

  ${CMAKE_SOURCE_DIR}/thirdparty/glm
  ${CMAKE_SOURCE_DIR}/thirdparty/SDL/include
  ${CMAKE_SOURCE_DIR}/thirdparty/bgfx.cmake/bgfx/include
  ${CMAKE_SOURCE_DIR}/thirdparty/bgfx.cmake/bx/include
)

target_link_libraries(engine PRIVATE SDL2 bgfx imgui bgfx-imgui-backend)

# Generate "engine_export.h" for DLL export
include(GenerateExportHeader)
generate_export_header(engine)

# Copy DLLs to output directory
add_custom_command(TARGET engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2>
        $<TARGET_FILE_DIR:engine>
)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(bgfx_quad
    main.cpp
)

target_include_directories(bgfx_quad PUBLIC
    ${CMAKE_SOURCE_DIR}/thirdparty/SDL/include
    ${CMAKE_SOURCE_DIR}/thirdparty/bgfx.cmake/bgfx/include
    ${CMAKE_SOURCE_DIR}/thirdparty/bgfx.cmake/bx/include
)

target_link_libraries(bgfx_quad PRIVATE SDL2main SDL2 bgfx)

# Copy DLLs to output directory
add_custom_command(TARGET bgfx_quad POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2>
        $<TARGET_FILE_DIR:bgfx_quad>

    # Vertex shader binary in project root
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/vs_quad.bin
        $<TARGET_FILE_DIR:bgfx_quad>

    # Fragment shader binary in project root
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/fs_quad.bin
        $<TARGET_FILE_DIR:bgfx_quad>
)
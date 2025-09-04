set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui)

file(GLOB IMGUI_SOURCES
    ${IMGUI_DIR}/*.cpp
    ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
    ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
    ${IMGUI_DIR}/
    ${IMGUI_DIR}/backends/
)

target_link_libraries(imgui PUBLIC SDL2)

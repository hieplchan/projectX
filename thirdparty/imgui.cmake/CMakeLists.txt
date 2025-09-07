set(IMGUI_DIR ${BGFX_DIR}/3rdparty/dear-imgui)

file(GLOB IMGUI_SOURCES
  ${IMGUI_DIR}/*.cpp
  # ${IMGUI_DIR}/backends/imgui_impl_sdl2.cpp
  # ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
  # ${CMAKE_CURRENT_SOURCE_DIR}
  # ${IMGUI_DIR}/backends/
  ${IMGUI_DIR}/
  ${IMGUI_DIR}/widgets/
  ${BGFX_DIR}/3rdparty/
)

target_link_libraries(imgui PUBLIC SDL2 bgfx bx bimg)

add_subdirectory(bgfx-imgui-helper)


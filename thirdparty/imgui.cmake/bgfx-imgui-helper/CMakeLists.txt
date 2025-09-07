file(GLOB BGFX_IMGUI_HELPER_SOURCES
    ${BGFX_DIR}/examples/common/imgui/*.cpp
)

add_library(bgfx-imgui-helper STATIC ${BGFX_IMGUI_HELPER_SOURCES})

target_include_directories(bgfx-imgui-helper PUBLIC
  ${BGFX_DIR}/examples/
  ${BGFX_DIR}/3rdparty/
)

target_link_libraries(bgfx-imgui-helper PUBLIC imgui bgfx bx bimg)

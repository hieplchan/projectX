# Source: https://github.com/MadLadSquad/UImGuiRendererExamples/tree/master/bgfx
# Source: https://gist.github.com/Madman10K/0ffc9ba203333d3580d467d972bbf8b6

file(GLOB BGFX_IMGUI_BACKEND_SOURCES
  ${IMGUI_DIR}/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui_impl_bgfx.cpp
)

add_library(bgfx-imgui-backend STATIC ${BGFX_IMGUI_BACKEND_SOURCES})

target_include_directories(bgfx-imgui-backend PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/
)

target_link_libraries(bgfx-imgui-backend PUBLIC imgui bgfx bx bimg)

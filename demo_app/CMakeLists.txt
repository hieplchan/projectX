add_executable(demo_app
    src/main.cpp
)

target_link_libraries(demo_app PRIVATE engine SDL2)

# Copy .dll
add_custom_command(TARGET demo_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2>
        $<TARGET_FILE_DIR:demo_app>
)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(demo_app
    src/main.cpp
)

set_target_properties(demo_app PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

target_include_directories(demo_app PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(demo_app PRIVATE engine nlohmann_json::nlohmann_json)

# Copy DLLs to output directory
add_custom_command(TARGET demo_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:engine>
        $<TARGET_FILE_DIR:demo_app>
)

# Copy assets folder to output directory
add_custom_command(TARGET demo_app POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/demo_app/assets
        $<TARGET_FILE_DIR:demo_app>/assets
)

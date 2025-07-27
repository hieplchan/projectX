cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(projectX)

if (MSVC)
    add_compile_options(/Zc:__cplusplus /Zc:preprocessor)
endif()

# Third party
add_subdirectory(thirdparty/SDL)
add_subdirectory(thirdparty/bgfx.cmake)

# Engine
add_subdirectory(engine)

# Application
add_subdirectory(demo_app)
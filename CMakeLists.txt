cmake_minimum_required(VERSION 3.15)
project(projectX)

# Third party
add_subdirectory(thirdparty/SDL)
add_subdirectory(thirdparty/bgfx.cmake)

# Engine
add_subdirectory(engine)

# Application
add_subdirectory(demo_app)
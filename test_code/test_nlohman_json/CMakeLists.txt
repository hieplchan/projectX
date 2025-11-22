cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(test_nlohmann_json
    main.cpp
)

target_link_libraries(test_nlohmann_json PRIVATE nlohmann_json::nlohmann_json)

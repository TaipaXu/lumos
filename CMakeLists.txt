cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

project(lumos VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_subdirectory(src)
add_subdirectory(package)

option(ENABLE_TESTS "Enable building tests" OFF)
if(ENABLE_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

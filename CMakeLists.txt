cmake_minimum_required(VERSION 3.17)

project(PhysicallyBasedRenderer
        VERSION 0.1
        DESCRIPTION "A library for physically based rendering using OpenGL")

set(CMAKE_CXX_STANDARD 17)

find_path(BOOST_FUNCTIONAL_INCLUDE_DIRS "boost/functional.hpp")
find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")
find_package(tinyobjloader CONFIG REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include/PBR)

add_subdirectory(src)
add_subdirectory(example)

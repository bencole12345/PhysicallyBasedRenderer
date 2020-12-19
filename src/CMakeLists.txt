cmake_minimum_required(VERSION 3.10)
project(PhysicallyBasedRenderer)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(renderer
        main.cpp
        core/Camera.cpp
        core/LightSource.cpp
        core/Scene.cpp
        core/ShaderProgram.cpp
        core/RendererWindow.cpp
        phong/BasicPhongScene.cpp
        phong/PhongSceneObject.cpp example/ExamplePhongSceneBuilder.cpp example/ExamplePhongSceneBuilder.h)

if (APPLE)
    target_link_libraries(renderer
        PRIVATE
            glfw
            GLEW::GLEW
            glm
        "-framework OpenGL")
else()
    target_link_libraries(renderer
        PRIVATE
            glfw
            GLEW::GLEW
            glm)
endif()

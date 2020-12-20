cmake_minimum_required(VERSION 3.10)
project(PhysicallyBasedRenderer)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(renderer
        main.cpp
        core/Camera.cpp
        core/PointLightSource.cpp
        core/Renderer.cpp
        core/RendererDriver.cpp
        core/ShaderProgram.cpp
        core/Window.cpp
        example/ExamplePhongSceneBuilder.cpp
        phong/PhongRenderer.cpp
        phong/PhongScene.cpp
        phong/PhongSceneObject.cpp)

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

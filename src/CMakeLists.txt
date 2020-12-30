cmake_minimum_required(VERSION 3.10)
project(PhysicallyBasedRenderer)

find_package(glfw3 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb.h")

add_executable(renderer
        main.cpp
        core/Camera.cpp
        core/PointLightSource.cpp
        core/Renderer.cpp
        core/RendererDriver.cpp
        core/Scene.cpp
        core/SceneObject.cpp
        core/ShaderProgram.cpp
        core/Skybox.cpp
        core/Texture.cpp
        core/VertexData.cpp
        core/Window.cpp
        example/ExampleSceneBuilder.cpp
        phong/PhongRenderer.cpp
        scene_objects/Cube.cpp
        scene_objects/Plane.cpp)

target_include_directories(renderer PRIVATE ${STB_INCLUDE_DIRS})

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

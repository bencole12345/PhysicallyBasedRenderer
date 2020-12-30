file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PhysicallyBasedRenderer_SOURCE_DIR}/include/**/*.h")

add_library(pbr
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
        phong/PhongRenderer.cpp
        scene_objects/Cube.cpp
        scene_objects/Plane.cpp
        ${HEADER_LIST})

target_include_directories(pbr
        PUBLIC
            ../include
        PRIVATE
            ${STB_INCLUDE_DIRS})

if (APPLE)
    target_link_libraries(pbr
        PUBLIC
            glfw
            GLEW::GLEW
            glm
        "-framework OpenGL")
else()
    target_link_libraries(pbr
        PUBLIC
            glfw
            GLEW::GLEW
            glm)
endif()

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)


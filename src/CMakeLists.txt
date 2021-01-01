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
        scene_objects/CustomObject.cpp
        scene_objects/Plane.cpp
        ${HEADER_LIST})

target_include_directories(pbr
        PUBLIC
            ../include
        PRIVATE
            ${STB_INCLUDE_DIRS})

set(PBR_PUBLIC_LINK_DEPENDENCIES
        glfw
        GLEW::GLEW
        glm
        tinyobjloader::tinyobjloader)

if (APPLE)
    list(APPEND PUBLIC_LINK_DEPENDENCIES "-framework OpenGL")
endif()

set(PBR_PRIVATE_LINK_DEPENDENCIES
        ${BOOST_FUNCTIONAL_INCLUDE_DIRS})

target_link_libraries(pbr PUBLIC ${PBR_PUBLIC_LINK_DEPENDENCIES}
                          PRIVATE ${PBR_PRIVATE_LINK_DEPENDENCIES})

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)


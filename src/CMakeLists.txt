file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${PhysicallyBasedRenderer_SOURCE_DIR}/include/**/*.h")

# Avoid "file has no symbols" warnings for files with nothing in the .cpp file on MacOS
if (APPLE)
    set(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
    set(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
    set(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
endif()

add_library(PBR
        core/Camera.cpp
        core/ErrorCodes.cpp
        core/PointLightSource.cpp
        core/Renderer.cpp
        core/RendererDriver.cpp
        core/Scene.cpp
        core/SceneObject.cpp
        core/ShaderProgram.cpp
        core/Texture.cpp
        core/VertexData.cpp
        core/Window.cpp
        debug/DebuggingUtil.cpp
        phong/PhongMaterial.cpp
        phong/PhongRenderer.cpp
        phong/PhongScene.cpp
        phong/PhongSceneObject.cpp
        phong/PhongShaderUniforms.cpp
        physically_based/EnvironmentMap.cpp
        physically_based/EnvironmentMapRenderer.cpp
        physically_based/FresnelValues.cpp
        physically_based/PhysicallyBasedMaterial.cpp
        physically_based/PhysicallyBasedRenderer.cpp
        physically_based/PhysicallyBasedScene.cpp
        physically_based/PhysicallyBasedSceneObject.cpp
        physically_based/PhysicallyBasedShaderUniforms.cpp
        physically_based/Util.cpp
        scene_objects/Cube.cpp
        scene_objects/CustomObject.cpp
        scene_objects/Plane.cpp
        scene_objects/Shapes.cpp
        skybox/Skybox.cpp
        skybox/SkyboxRenderer.cpp
        ${HEADER_LIST})

target_compile_features(PBR PRIVATE cxx_std_17)

target_include_directories(PBR
        PUBLIC
                ../include
                ${GLM_INCLUDE_DIRS}
        PRIVATE
                ${STB_INCLUDE_DIRS}
                ${OPENGL_INCLUDE_DIR}
                ${GLEW_INCLUDE_DIRS}
                ${BOOST_FUNCTIONAL_INCLUDE_DIRS})

set(PBR_PUBLIC_LINK_DEPENDENCIES
        glm)

if (APPLE)
        list(APPEND PUBLIC_LINK_DEPENDENCIES "-framework OpenGL")
endif()

set(PBR_PRIVATE_LINK_DEPENDENCIES
        glfw
        GLEW::GLEW
        ${GLEW_LIBRARIES}
        tinyobjloader::tinyobjloader)

target_link_libraries(PBR PUBLIC ${PBR_PUBLIC_LINK_DEPENDENCIES}
                          PRIVATE ${PBR_PRIVATE_LINK_DEPENDENCIES})

source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)


#include "physically_based/PhysicallyBasedRenderer.h"

#include <filesystem>
#include <memory>

#include <GL/glew.h>

#include "physically_based/PBRUtil.h"
#include "physically_based/PhysicallyBasedScene.h"
#include "physically_based/PhysicallyBasedShaderUniforms.h"

namespace fs = std::filesystem;

namespace PBR::physically_based {

namespace {

const fs::path& vertexShaderPath()
{
    static fs::path path = PBRUtil::pbrShadersDir() / "PhysicallyBasedShader.vert";
    return path;
}

const fs::path& fragmentShaderPath()
{
    static fs::path path = PBRUtil::pbrShadersDir() / "PhysicallyBasedShader.frag";
    return path;
}

} // anonymous namespace

PhysicallyBasedRenderer::PhysicallyBasedRenderer()
        :shaderProgram(vertexShaderPath(), fragmentShaderPath()), environmentMapRenderer()
{
}

void PhysicallyBasedRenderer::activate()
{
    // Use the Z buffer
    glEnable(GL_DEPTH_TEST);

    // Cull faces oriented the wrong way for performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void PhysicallyBasedRenderer::render(std::shared_ptr<PhysicallyBasedScene> scene, const Camera& camera, double time)
{
    // Enable the shader program
    glUseProgram(shaderProgram.id());

    // Render each object in the scene
    for (size_t i = 0; i < scene->getSceneObjectsList().size(); i++) {

        const auto& object = scene->getSceneObjectsList()[i];
        const auto& brdfIntegrationMap = scene->getBRDFIntegrationMaps()[i];

        // Write the uniforms to the shader
        PhysicallyBasedShaderUniforms uniforms{
                object->getModelMatrix(),
                camera.getViewMatrix(),
                camera.getProjectionMatrix(),
                object->getRotationMatrix(),
                camera.position(),
                object->material,
                PhysicallyBasedDirectLightingInfo{scene->getLightPositions(), scene->getLightColours(),
                                                  scene->getLightIntensities()},
                scene->getEnvironmentMap()->getSun(),
                scene->getEnvironmentMap()->getIrradianceMap(),
                scene->getEnvironmentMap()->getPreFilteredEnvironmentMap(),
                brdfIntegrationMap,
        };
        writeUniformsToShaderProgram(uniforms, shaderProgram);

        // Draw the object
        glBindVertexArray(object->vertexData->getVaoId());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->vertexData->getEboId());
        glDrawElements(GL_TRIANGLES, object->vertexData->verticesCount(), GL_UNSIGNED_INT, (void*) 0);

        // Reset the uniforms ready for the next usage
        shaderProgram.resetUniforms();
    }

    // Render the environment map as a skybox
    environmentMapRenderer.renderSkybox(scene->getEnvironmentMap(), camera);
}

} // namespace PBR::physically_based

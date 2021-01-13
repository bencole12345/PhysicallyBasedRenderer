#include "physically_based/PhysicallyBasedRenderer.h"

#include <memory>
#include <string_view>

#include <GL/glew.h>

#include "physically_based/PhysicallyBasedScene.h"
#include "physically_based/PhysicallyBasedShaderUniforms.h"

namespace {

constexpr std::string_view PHYSICALLY_BASED_VERTEX_SHADER = "src/physically_based/shaders/physically_based.vert";
constexpr std::string_view PHYSICALLY_BASED_FRAGMENT_SHADER = "src/physically_based/shaders/physically_based.frag";

} // anonymous namespace

namespace PBR::physically_based {

PhysicallyBasedRenderer::PhysicallyBasedRenderer()
        :shaderProgram(PHYSICALLY_BASED_VERTEX_SHADER, PHYSICALLY_BASED_FRAGMENT_SHADER), environmentMapRenderer()
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
    for (const auto& object : scene->getSceneObjectsList()) {

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
                scene->getEnvironmentMap()->getDiffuseIrradianceMapTexture(),
                scene->getEnvironmentMap()->getSpecularIrradianceMapTexture(),
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

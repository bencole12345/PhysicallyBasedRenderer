#include "physically_based/PhysicallyBasedRenderer.h"

#include <memory>
#include <optional>
#include <string_view>

#include "physically_based/PhysicallyBasedScene.h"
#include "physically_based/PhysicallyBasedShaderUniforms.h"

namespace {

constexpr std::string_view PHYSICALLY_BASED_VERTEX_SHADER = "src/physically_based/shaders/physically_based.vert";
constexpr std::string_view PHYSICALLY_BASED_FRAGMENT_SHADER = "src/physically_based/shaders/physically_based.frag";

} // anonymous namespace

namespace PBR::physically_based {

PhysicallyBasedRenderer::PhysicallyBasedRenderer()
    : shaderProgram(PHYSICALLY_BASED_VERTEX_SHADER, PHYSICALLY_BASED_FRAGMENT_SHADER),
      skyboxRenderer()
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

        shaderProgram.resetUniforms();

        // Write the uniforms to the shader
        PhysicallyBasedShaderUniforms uniforms{
                .modelMatrix = object->getModelMatrix(),
                .viewMatrix = camera.getViewMatrix(),
                .projectionMatrix = camera.getProjectionMatrix(),
                .cameraPosition = camera.position(),
                .material = object->material,
                .lightingInfo = PhysicallyBasedDirectLightingInfo{
                        .lightPositions = scene->getLightPositions(),
                        .lightColours = scene->getLightColours(),
                        .intensities = scene->getLightIntensities()
                },
                .skybox = scene->hasSkybox() ? std::optional(scene->getSkybox()) : std::nullopt
        };
        writeUniformsToShaderProgram(uniforms, shaderProgram);

        // Draw the object
        glBindVertexArray(object->vertexData->getVaoId());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->vertexData->getEboId());
        glDrawElements(GL_TRIANGLES, object->vertexData->verticesCount(), GL_UNSIGNED_INT, (void*)0);
    }

    // Render the skybox, if the scene has one
    if (scene->hasSkybox()) {
        skyboxRenderer.renderSkybox(scene->getSkybox(), camera);
    }
}

} // namespace PBR::physically_based

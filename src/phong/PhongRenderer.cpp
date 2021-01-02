#include "phong/PhongRenderer.h"

#include <optional>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "core/Camera.h"
#include "core/Scene.h"
#include "core/ShaderProgram.h"
#include "phong/PhongShaderUniforms.h"

#define TEXTURED_OBJECT_VERTEX_SHADER "src/shaders/phong/phong_textured.vert"
#define TEXTURED_OBJECT_FRAGMENT_SHADER "src/shaders/phong/phong_textured.frag"
#define UNTEXTURED_OBJECT_VERTEX_SHADER "src/shaders/phong/phong_untextured.vert"
#define UNTEXTURED_OBJECT_FRAGMENT_SHADER "src/shaders/phong/phong_untextured.frag"

namespace PBR::phong {

PhongRenderer::PhongRenderer()
    : texturedObjectShader(TEXTURED_OBJECT_VERTEX_SHADER, TEXTURED_OBJECT_FRAGMENT_SHADER),
      nonTexturedObjectShader(UNTEXTURED_OBJECT_VERTEX_SHADER, UNTEXTURED_OBJECT_FRAGMENT_SHADER)
{
}

void PhongRenderer::activate()
{
    // Use the Z buffer
    glEnable(GL_DEPTH_TEST);

    // Cull faces oriented the wrong way for performance
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void PhongRenderer::render(std::shared_ptr<Scene> scene, const Camera& camera, double time)
{
    // Render each object in the scene
    for (const auto& object : scene->getSceneObjectsList()) {

        // Select the shader program that we are going to use
        ShaderProgram& shaderProgram = object->hasTexture()
                ? texturedObjectShader
                : nonTexturedObjectShader;

        // Enable the shader program
        glUseProgram(shaderProgram.id());

        // Write the uniforms to the shader
        PhongShaderUniforms uniforms{
            .modelMatrix = object->getModelMatrix(),
            .viewMatrix = camera.getViewMatrix(),
            .projectionMatrix = camera.getProjectionMatrix(),
            .cameraPosition = camera.position(),
            .material = object->material,
            .lightingInfo = LightingInfo{
                .ambientLight = scene->getAmbientLight(),
                .pointLightPositions = scene->getLightPositions(),
                .pointLightColours = scene->getLightColours()
            },
            .textureId = object->hasTexture() ? std::optional<unsigned int>(object->texture->get()->id()) : std::nullopt,
            .colour = object->hasConstantColour() ? std::optional<glm::vec3>(object->colour.value()) : std::nullopt
        };
        writeUniformsToShaderProgram(uniforms, shaderProgram);

        // Draw the object
        glBindVertexArray(object->vertexData->getVaoId());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->vertexData->getEboId());
        glDrawElements(GL_TRIANGLES, object->vertexData->verticesCount(), GL_UNSIGNED_INT, (void*)0);
    }

    // Render the skybox, if the scene has one
    if (scene->hasSkybox()) {
        scene->getSkybox()->draw(camera);
    }
}

} // namespace PBR

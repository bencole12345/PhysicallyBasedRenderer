#include "phong/PhongRenderer.h"

#include <string>
#include <utility>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "core/Camera.h"
#include "core/Scene.h"
#include "core/ShaderProgram.h"

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

        // Use the shader program
        ShaderProgram& shaderProgram = object->hasTexture()
                ? texturedObjectShader
                : nonTexturedObjectShader;
        glUseProgram(shaderProgram.id());

        // If the object has a texture, bind that too
        if (object->hasTexture()) {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, object->texture->get()->id());
            // TODO: Give this a better name
            shaderProgram.setUniform("myTexture", 0);
        }

        // Compute the matrices we need to pass
        const glm::mat4 modelMatrix = object->getModelMatrix();
        const glm::mat4 viewMatrix = camera.getViewMatrix();
        const glm::mat4 projectionMatrix = camera.getProjectionMatrix();

        // Additional values that need to be passed in
        const glm::vec4 cameraPosition(camera.position(), 1.0f);
        const glm::vec4 ambientLight(scene->getAmbientLight(), 1.0f);

        // Set all the uniforms
        // TODO: Use a struct that holds all of these
        shaderProgram.setUniform("Model", modelMatrix);
        shaderProgram.setUniform("View", viewMatrix);
        shaderProgram.setUniform("Projection", projectionMatrix);
        shaderProgram.setUniform("cameraPosition", cameraPosition);
        shaderProgram.setUniform("time", time);
        shaderProgram.setUniform("material.kD", object->material.kD);
        shaderProgram.setUniform("material.kS", object->material.kS);
        shaderProgram.setUniform("material.specularN", object->material.specularN);
        shaderProgram.setUniform("ambientLight", ambientLight);
        shaderProgram.setUniform("lights.positions", scene->getLightPositions());
        shaderProgram.setUniform("lights.colours", scene->getLightColours());

        if (object->hasConstantColour()) {
            shaderProgram.setUniform("MaterialColour", object->colour.value());
        }

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

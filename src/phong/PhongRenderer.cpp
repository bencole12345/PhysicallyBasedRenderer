#include "phong/PhongRenderer.h"

#include <optional>
#include <string_view>

#include <GL/glew.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

#include "core/Camera.h"
#include "core/Scene.h"
#include "core/ShaderProgram.h"
#include "phong/PhongScene.h"
#include "phong/PhongShaderUniforms.h"
#include "skybox/Skybox.h"
#include "skybox/SkyboxRenderer.h"

namespace {

constexpr std::string_view TEXTURED_OBJECT_VERTEX_SHADER = "src/phong/shaders/phong_textured.vert";
constexpr std::string_view TEXTURED_OBJECT_FRAGMENT_SHADER = "src/phong/shaders/phong_textured.frag";
constexpr std::string_view UNTEXTURED_OBJECT_VERTEX_SHADER = "src/phong/shaders/phong_untextured.vert";
constexpr std::string_view UNTEXTURED_OBJECT_FRAGMENT_SHADER = "src/phong/shaders/phong_untextured.frag";

} // anonymous namespace

namespace PBR::phong {

PhongRenderer::PhongRenderer()
        :texturedObjectShader(TEXTURED_OBJECT_VERTEX_SHADER, TEXTURED_OBJECT_FRAGMENT_SHADER),
         nonTexturedObjectShader(UNTEXTURED_OBJECT_VERTEX_SHADER, UNTEXTURED_OBJECT_FRAGMENT_SHADER),
         skyboxRenderer()
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

void PhongRenderer::render(std::shared_ptr<PhongScene> scene, const Camera& camera, double time)
{
    // Render each object in the scene
    for (const auto& object : scene->getSceneObjectsList()) {

        assert((object->hasTexture() || object->material.colour.has_value())
                       && "Objects must either have a colour or texture.");

        // Select the shader program that we are going to use
        ShaderProgram& shaderProgram = object->hasTexture()
                                       ? texturedObjectShader
                                       : nonTexturedObjectShader;

        // Enable the shader program
        glUseProgram(shaderProgram.id());

        // Write the uniforms to the shader
        PhongShaderUniforms uniforms{
                object->getModelMatrix(),
                camera.getViewMatrix(),
                camera.getProjectionMatrix(),
                object->getRotationMatrix(),
                camera.position(),
                object->material,
                LightingInfo{scene->getAmbientLight(), scene->getLightPositions(), scene->getLightColours()},
                object->hasTexture() ? std::optional<unsigned int>(object->texture->get()->id()) : std::nullopt,
        };
        writeUniformsToShaderProgram(uniforms, shaderProgram);

        // Draw the object
        glBindVertexArray(object->vertexData->getVaoId());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object->vertexData->getEboId());
        glDrawElements(GL_TRIANGLES, object->vertexData->verticesCount(), GL_UNSIGNED_INT, (void*) 0);
    }

    // Render the skybox, if the scene has one
    if (scene->hasSkybox()) {
        skyboxRenderer.renderSkybox(scene->getSkybox(), camera);
    }
}

} // namespace PBR

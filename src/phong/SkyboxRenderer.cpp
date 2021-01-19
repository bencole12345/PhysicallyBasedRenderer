#include "phong/SkyboxRenderer.h"

#include <memory>
#include <string>
#include <string_view>

#include <GL/glew.h>

#include "core/Camera.h"
#include "phong/Skybox.h"

namespace {

constexpr std::string_view SKYBOX_VERTEX_SHADER = "src/phong/shaders/skybox.vert";
constexpr std::string_view SKYBOX_FRAGMENT_SHADER = "src/phong/shaders/skybox.frag";

} // anonymous namespace

namespace PBR::phong {

SkyboxRenderer::SkyboxRenderer()
    : shaderProgram(SKYBOX_VERTEX_SHADER, SKYBOX_FRAGMENT_SHADER)
{
}

void SkyboxRenderer::renderSkybox(const std::shared_ptr<Skybox>& skybox, const PBR::Camera& camera)
{
    // Use the shader program
    glUseProgram(shaderProgram.id());

    // Pass in the view and projection matrices
    const auto viewMatrix = camera.getViewMatrix();
    const auto projectionMatrix = camera.getProjectionMatrix();

    // Only do the rotation part of the view matrix's transform
    auto viewMatrixCorrected = glm::mat4(glm::mat3(viewMatrix));

    shaderProgram.setUniform("View", viewMatrixCorrected);
    shaderProgram.setUniform("Projection", projectionMatrix);

    // Save the old depth culling mode
    GLint oldDepthFunc;
    GLint oldCullFace;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFace);

    // We need less than or equal, because both the skybox and the unrendered
    // fragments will be at the maximum z-buffer value.
    glDepthFunc(GL_LEQUAL);

    // We show the front rather than the back because we're inside the cube now.
    glCullFace(GL_FRONT);

    // Render the skybox
    glBindVertexArray(skybox->getVaoId());
    glDrawArrays(GL_TRIANGLES, 0, skybox->numVerticesInBuffer());

    // Restore the old face culling settings.
    glDepthFunc(oldDepthFunc);
    glCullFace(oldCullFace);
}

} // namespace PBR::phong

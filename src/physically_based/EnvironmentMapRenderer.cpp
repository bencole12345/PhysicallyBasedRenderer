#include "physically_based/EnvironmentMapRenderer.h"

#include <filesystem>
#include <memory>

#include <GL/glew.h>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>

#include "core/Camera.h"
#include "core/ShaderProgram.h"
#include "physically_based/EnvironmentMap.h"
#include "physically_based/PBRUtil.h"

namespace fs = std::filesystem;

namespace PBR::physically_based {

namespace {

const fs::path& getSkyboxVertexShaderPath()
{
    static fs::path path = PBRUtil::pbrShadersDir() / "RenderHDRSkybox.vert";
    return path;
}

const fs::path& getSkyboxFragmentShaderPath()
{
    static fs::path path = PBRUtil::pbrShadersDir() / "RenderHDRSkybox.frag";
    return path;
}

constexpr float skyboxCubeVertices[] = {
        // Position(x, y, z)

        // Back face
        0.5f, 0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,

        // Front face
        -0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        // Left face
        -0.5f, 0.5f, 0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, -0.5f,
        -0.5f, -0.5f, 0.5f,
        -0.5f, 0.5f, 0.5f,

        // Right face
        0.5f, -0.5f, -0.5f,
        0.5f, 0.5f, -0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
        0.5f, -0.5f, 0.5f,
        0.5f, -0.5f, -0.5f,

        // Bottom face
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, -0.5f,
        0.5f, -0.5f, 0.5f,
        -0.5f, -0.5f, 0.5f,

        // Top face
        0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, -0.5f,
        -0.5f, 0.5f, 0.5f,
        0.5f, 0.5f, 0.5f,
};

} // anonymous namespace

EnvironmentMapRenderer::EnvironmentMapRenderer()
    :skyboxRenderingShader(getSkyboxVertexShaderPath(), getSkyboxFragmentShaderPath()), vaoId(), vboId()
{
    // Create a vertex array buffer object
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // Create a vertex buffer object
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);

    // Copy the vertex data over to the GPU
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxCubeVertices), skyboxCubeVertices, GL_STATIC_DRAW);

    // Set upt the vertex attribute array
    size_t stride = 3 * sizeof(float);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);

    // Unbind now that we're done
    glEnableVertexAttribArray(0);
}

EnvironmentMapRenderer::~EnvironmentMapRenderer()
{
    glDeleteBuffers(1, &vboId);
    glDeleteVertexArrays(1, &vaoId);
}

void EnvironmentMapRenderer::renderSkybox(const std::shared_ptr<EnvironmentMap>& environmentMap, const Camera& camera)
{
    // Use our shader
    glUseProgram(skyboxRenderingShader.id());

    // Compute uniform matrices
    const auto viewMatrix = camera.getViewMatrix();
    const auto projectionMatrix = camera.getProjectionMatrix();

    // Correct the view matrix because we only want rotation, no translation
    const auto viewMatrixCorrected = glm::mat4(glm::mat3(viewMatrix));

    // Pass in uniforms
    skyboxRenderingShader.resetUniforms();
    skyboxRenderingShader.setUniform("View", viewMatrixCorrected);
    skyboxRenderingShader.setUniform("Projection", projectionMatrix);
    skyboxRenderingShader.setUniform("SkyboxTexture", environmentMap->getRadianceMap());

    // We need to change the depth culling mode for this render. We need to use
    // less-than-or-equal so that unrendered values beyond the far clipping plane
    // get overwritten by the skybox. We also change the culled face because we're
    // now inside the cube, not outside like we usually are when we render meshes.
    GLint oldDepthFunc, oldCullFace;
    glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFunc);
    glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFace);
    glDepthFunc(GL_LEQUAL);
    glCullFace(GL_FRONT);

    // Execute the shader program
    glBindVertexArray(vaoId);
    int count = 36; // Number of vertices to draw
    glDrawArrays(GL_TRIANGLES, 0, count);

    // Restore the old settings
    glDepthFunc(oldDepthFunc);
    glCullFace(oldCullFace);

    skyboxRenderingShader.resetUniforms();
}

} // namespace PBR::physically_based

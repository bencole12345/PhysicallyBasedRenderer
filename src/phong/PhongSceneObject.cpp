#include "phong/PhongSceneObject.h"

#include <string>
#include <memory>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PBR {
namespace phong {

PhongSceneObject::PhongSceneObject(const float* vertices, const size_t vertexBufferLen,
        std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 pos, glm::vec3 orientation, float scale, float kD,
        float kS, float specularN)
        :shaderProgram(std::move(shaderProgram)), pos(pos), orientation(orientation), scale(scale), kD(kD), kS(kS),
         specularN(specularN),
         vertices(vertices), vertexBufferLen(vertexBufferLen), vaoId(), vboId(),
         hasTexture(false), texture({})
{
    initBuffers();
}

PhongSceneObject::PhongSceneObject(const float* vertices, size_t vertexBufferLen, std::shared_ptr<Texture> texture,
        std::shared_ptr<ShaderProgram> shaderProgram, glm::vec3 pos, glm::vec3 orientation, float scale, float kD,
        float kS, float specularN)
        :shaderProgram(std::move(shaderProgram)), pos(pos), orientation(orientation), scale(scale), kD(kD), kS(kS),
         specularN(specularN),
         vertices(vertices), vertexBufferLen(vertexBufferLen), vaoId(), vboId(),
         hasTexture(true), texture(texture)
{
    initBuffers();
}

void PhongSceneObject::initBuffers()
{
    // The vertex array object
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // The vertex buffer object
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferLen, vertices, GL_STATIC_DRAW);

    // Bind the buffers
    size_t stride = vertexBufferStrideLength();

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);

    // Vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    if (hasTexture) {
        // Texture coordinates
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
    else {
        // Vertex colours
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (6 * sizeof(float)));
        glEnableVertexAttribArray(2);
    }
}

// TODO: Add destructor that deletes the buffers we created

void PhongSceneObject::render(
        const Camera& camera,
        double time,
        glm::vec3 ambientLight,
        const std::vector<glm::vec3>& lightPositions,
        const std::vector<glm::vec3>& lightColours)
{
    glUseProgram(shaderProgram->id());

    // Bind the array of vertex positions
    glBindVertexArray(vaoId);

    // Bind the texture, if this object has one
    if (hasTexture) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, (*texture)->id());
        glUniform1i(glGetUniformLocation(shaderProgram->id(), "myTexture"), 0);
    }

    // Compute the matrices we need to pass to the shader program
    const glm::mat4 modelMatrix = getModelMatrix();
    const glm::mat4 viewMatrix = camera.getViewMatrix();
    const glm::mat4 projectionMatrix = camera.getProjectionMatrix();

    // Pass in camera information
    shaderProgram->setUniform("Model", modelMatrix);
    shaderProgram->setUniform("View", viewMatrix);
    shaderProgram->setUniform("Projection", projectionMatrix);
    shaderProgram->setUniform("cameraPosition", glm::vec4(camera.position(), 1.0f));

    // Pass in the current time
    shaderProgram->setUniform("time", time);

    // Pass in the material information
    shaderProgram->setUniform("material.kD", kD);
    shaderProgram->setUniform("material.kS", kS);
    shaderProgram->setUniform("material.specularN", specularN);

    // Pass in light information
    shaderProgram->setUniform("ambientLight", glm::vec4(ambientLight, 1.0f));
    shaderProgram->setUniform("lights.positions", lightPositions);
    shaderProgram->setUniform("lights.colours", lightColours);

    // Draw the object
    size_t stride = vertexBufferStrideLength();
    glDrawArrays(GL_TRIANGLES, 0, vertexBufferLen / stride);
}

glm::mat4 PhongSceneObject::getModelMatrix()
{
    // Remember that these are essentially in reverse order. We start with
    // an identity matrix and apply the transformations to it in
    // last-to-first order.

    glm::mat4 identity(1.0f);

    // Last do the translation
    auto translated = glm::translate(identity, pos);

    // Before that, apply the scaling, uniformly in all dimensions.
    auto scaled = glm::scale(translated, glm::vec3(scale));

    // First we do the rotations.
    auto rotatedZ = glm::rotate(scaled, orientation[2], glm::vec3(0.0f, 0.0f, 1.0f));
    auto rotatedYZ = glm::rotate(rotatedZ, orientation[1], glm::vec3(0.0f, 1.0f, 0.0f));
    auto rotatedXYZ = glm::rotate(rotatedYZ, orientation[0], glm::vec3(1.0f, 0.0f, 0.0f));

    return rotatedXYZ;
}

size_t PhongSceneObject::vertexBufferStrideLength() const
{
    if (hasTexture) {
        // 3 (positions) + 3 (normals) + 2 (texture coordinates)
        return 8 * sizeof(float);
    }
    else {
        // 3 (positions) + 3 (normals) + 3 (colours)
        return 9 * sizeof(float);
    }
}

} // namespace phong
} // namespace PBR

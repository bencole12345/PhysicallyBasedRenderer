#include "PhongSceneObject.h"

#include <string>

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PBR {

PhongSceneObject::PhongSceneObject(const float* vertices, const size_t vertexBufferLen,
        std::shared_ptr<ShaderProgram> shaderProgram, float kD, float kS, float specularN)
        :shaderProgram(std::move(shaderProgram)), kD(kD), kS(kS), specularN(specularN),
         vertices(vertices), vertexBufferLen(vertexBufferLen), vaoId(), vboId()
{
    // The vertex array object
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // The vertex buffer object
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, vertexBufferLen, vertices, GL_STATIC_DRAW);

    // Binding the buffers
    size_t stride = 9 * sizeof(float);  // 3 (vertex positions) + 3 (normals) + 3 (colours)

    // Vertex positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*) 0);
    glEnableVertexAttribArray(0);

    // Vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Vertex colours
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*) (6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

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
    glDrawArrays(GL_TRIANGLES, 0, vertexBufferLen / 9);
}

glm::mat4 PhongSceneObject::getModelMatrix()
{
    glm::mat4 identity(1.0f);
    return identity;
}

} // namespace PBR

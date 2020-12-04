#include "SceneObject.h"

#define GL_SILENCE_DEPRECATION
#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace PBR {

SceneObject::SceneObject(const float* vertices, const size_t verticesCount, const ShaderProgram& shaderProgram)
        :shaderProgram(shaderProgram), vertices(vertices), verticesCount(verticesCount), vaoId(), vboId()
{
    // The vertex array object
    glGenVertexArrays(1, &vaoId);
    glBindVertexArray(vaoId);

    // The vertex buffer object
    glGenBuffers(1, &vboId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, verticesCount, vertices, GL_STATIC_DRAW);

    // Binding the buffers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) nullptr);
    glEnableVertexAttribArray(0);
}

void SceneObject::render(glm::mat4 mvpMatrix) const
{
    glUseProgram(shaderProgram.id());

    // Bind the array of vertex positions
    glBindVertexArray(vaoId);

    // Pass in the current time
    int timePosition = glGetUniformLocation(shaderProgram.id(), "time");
    glUniform1d(timePosition, glfwGetTime());

    // Pass in the MVP matrix
    int mvpPosition = glGetUniformLocation(shaderProgram.id(), "MVP");
    glUniformMatrix4fv(mvpPosition, 1, GL_FALSE, &mvpMatrix[0][0]);

    // Draw the object
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

} // namespace PBR

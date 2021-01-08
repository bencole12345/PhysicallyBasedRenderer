#include "phong/PhongShaderUniforms.h"

#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

namespace PBR::phong {

void writeUniformsToShaderProgram(const PhongShaderUniforms& uniforms, ShaderProgram& shaderProgram)
{
    shaderProgram.setUniform("Model", uniforms.modelMatrix);
    shaderProgram.setUniform("View", uniforms.viewMatrix);
    shaderProgram.setUniform("Projection", uniforms.projectionMatrix);
    shaderProgram.setUniform("cameraPosition", uniforms.cameraPosition);
    shaderProgram.setUniform("material.kD", uniforms.material.kD);
    shaderProgram.setUniform("material.kS", uniforms.material.kS);
    shaderProgram.setUniform("material.specularN", uniforms.material.specularN);
    shaderProgram.setUniform("lightingInfo.ambientLight", uniforms.lightingInfo.ambientLight);
    shaderProgram.setUniform("lightingInfo.lightPositions", uniforms.lightingInfo.pointLightPositions);
    shaderProgram.setUniform("lightingInfo.pointLightColours", uniforms.lightingInfo.pointLightColours);

    if (uniforms.material.colour.has_value()) {
        shaderProgram.setUniform("surfaceColour", uniforms.material.colour.value());
    }

    if (uniforms.textureId) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, uniforms.textureId.value());
        shaderProgram.setUniform("surfaceTexture", 0);
    }
}

} // namespace PBR::phong

#include "physically_based/PhysicallyBasedShaderUniforms.h"

#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>

namespace PBR::physically_based {

void writeUniformsToShaderProgram(const PhysicallyBasedShaderUniforms& uniforms, ShaderProgram& shaderProgram)
{
    shaderProgram.setUniform("Model", uniforms.modelMatrix);
    shaderProgram.setUniform("View", uniforms.viewMatrix);
    shaderProgram.setUniform("Projection", uniforms.projectionMatrix);
    shaderProgram.setUniform("cameraPosition", uniforms.cameraPosition);
    shaderProgram.setUniform("material.albedo", uniforms.material.albedo);
    shaderProgram.setUniform("material.roughness", uniforms.material.roughness);
    shaderProgram.setUniform("material.metallic", uniforms.material.metallic);
    shaderProgram.setUniform("lightingInfo.ambientLight", uniforms.lightingInfo.ambientLight);
    shaderProgram.setUniform("lightingInfo.pointLightPositions", uniforms.lightingInfo.pointLightPositions);
    shaderProgram.setUniform("lightingInfo.pointLightColours", uniforms.lightingInfo.pointLightColours);
    shaderProgram.setUniform("skybox", uniforms.skybox);
}

} // namespace PBR::physically_based

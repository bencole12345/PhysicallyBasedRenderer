#include "physically_based/PhysicallyBasedShaderUniforms.h"

#include <string>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

namespace PBR::physically_based {

void writeUniformsToShaderProgram(const PhysicallyBasedShaderUniforms& uniforms, ShaderProgram& shaderProgram)
{
    // The matrices for rendering
    shaderProgram.setUniform("Model", uniforms.modelMatrix);
    shaderProgram.setUniform("View", uniforms.viewMatrix);
    shaderProgram.setUniform("Projection", uniforms.projectionMatrix);

    // The camera's position in world coordinates
    shaderProgram.setUniform("cameraPosition", uniforms.cameraPosition);

    // Material properties
    shaderProgram.setUniform("material.albedo", uniforms.material.albedo);
    shaderProgram.setUniform("material.roughness", uniforms.material.roughness);
    shaderProgram.setUniform("material.metallic", uniforms.material.metallic);
    shaderProgram.setUniform("material.F0", uniforms.material.F0);

    // Direct lighting information
    shaderProgram.setUniform("lightingInfo.lightPositions", uniforms.lightingInfo.lightPositions);
    shaderProgram.setUniform("lightingInfo.lightColours", uniforms.lightingInfo.lightColours);
    shaderProgram.setUniform("lightingInfo.intensities", uniforms.lightingInfo.intensities);

    // Skybox
    if (uniforms.skybox) {
        shaderProgram.setUniform("skybox", uniforms.skybox.value());
    }
}

} // namespace PBR::physically_based

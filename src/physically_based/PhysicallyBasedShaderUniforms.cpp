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

    // The matrix for rotating normals
    shaderProgram.setUniform("NormalsRotation", uniforms.normalsRotationMatrix);

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

    // Irradiance maps
    shaderProgram.setUniform("diffuseIrradianceMap", uniforms.diffuseIrradianceMap);
    shaderProgram.setUniform("specularIrradianceMap", uniforms.specularIrradianceMap);

    // The sun, if present
    if (uniforms.sun) {
        shaderProgram.setUniform("sunInfo.direction", uniforms.sun->direction);
        shaderProgram.setUniform("sunInfo.colour", uniforms.sun->colour);
        shaderProgram.setUniform("sunInfo.intensity", uniforms.sun->intensity);
    }
    else {
        // We have to send something anyway to avoid undefined behaviour
        shaderProgram.setUniform("sunInfo.direction", glm::vec3(1.0f, 0.0f, 0.0f));
        shaderProgram.setUniform("sunInfo.colour", glm::vec3(1.0f));
        shaderProgram.setUniform("sunInfo.intensity", 0.0f);
    }
}

} // namespace PBR::physically_based

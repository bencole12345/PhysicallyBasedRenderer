#ifndef PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSHADERUNIFORMS
#define PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSHADERUNIFORMS

#include <memory>
#include <optional>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "core/DirectedLightSource.h"
#include "core/ShaderProgram.h"
#include "core/Texture.h"
#include "physically_based/BRDFCoefficients.h"
#include "physically_based/PhysicallyBasedMaterial.h"

namespace PBR::physically_based {

struct PhysicallyBasedDirectLightingInfo {
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColours;
    std::vector<float> intensities;
};

struct PhysicallyBasedShaderUniforms {

    // Geometry stuff
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 normalsRotationMatrix;
    glm::vec3 cameraPosition;

    // Material description
    PhysicallyBasedMaterial material;

    // Lighting information
    PhysicallyBasedDirectLightingInfo lightingInfo;
    std::optional<DirectedLightSource> sun;

    // Distribution coefficients;
    NormalDistributionFunctionCoefficients ndfCoefficients;
    GeometricAttenuationFunctionCoefficients geometryCoefficients;

    // Lighting maps
    std::shared_ptr<Texture> irradianceMap;
    std::shared_ptr<Texture> preFilteredEnvironmentMap;
    std::shared_ptr<Texture> brdfIntegrationMap;
};

void writeUniformsToShaderProgram(const PhysicallyBasedShaderUniforms& uniforms, ShaderProgram& shaderProgram);

} // namespace PBR::phong

#endif //PHYSICALLYBASEDRENDERER_PHONGSHADERUNIFORMS

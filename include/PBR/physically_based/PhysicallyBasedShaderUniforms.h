#ifndef PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSHADERUNIFORMS
#define PHYSICALLYBASEDRENDERER_PHYSICALLYBASEDSHADERUNIFORMS

#include <memory>
#include <optional>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "physically_based/PhysicallyBasedMaterial.h"
#include "core/ShaderProgram.h"
#include "skybox/Skybox.h"
#include "core/Texture.h"

namespace PBR::physically_based {

struct PhysicallyBasedDirectLightingInfo {
    std::vector<glm::vec3> lightPositions;
    std::vector<glm::vec3> lightColours;
    std::vector<float> intensities;
};

struct PhysicallyBasedShaderUniforms {
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::mat4 normalsRotationMatrix;
    glm::vec3 cameraPosition;
    PhysicallyBasedMaterial material;
    PhysicallyBasedDirectLightingInfo lightingInfo;
    std::optional<std::shared_ptr<skybox::Skybox>> skybox {std::nullopt};
};

void writeUniformsToShaderProgram(const PhysicallyBasedShaderUniforms& uniforms, ShaderProgram& shaderProgram);

} // namespace PBR::phong

#endif //PHYSICALLYBASEDRENDERER_PHONGSHADERUNIFORMS

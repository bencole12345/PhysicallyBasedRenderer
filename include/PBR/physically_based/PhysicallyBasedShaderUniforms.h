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

struct PhysicallyBasedLightingInfo {
    glm::vec3 ambientLight;
    std::vector<glm::vec3> pointLightPositions;
    std::vector<glm::vec3> pointLightColours;
};

struct PhysicallyBasedShaderUniforms {
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 cameraPosition;
    PhysicallyBasedMaterial material;
    PhysicallyBasedLightingInfo lightingInfo;
//    std::optional<std::shared_ptr<Texture>> texture;

    // New stuff!
    std::shared_ptr<skybox::Skybox> skybox;

};

void writeUniformsToShaderProgram(const PhysicallyBasedShaderUniforms& uniforms, ShaderProgram& shaderProgram);

} // namespace PBR::phong

#endif //PHYSICALLYBASEDRENDERER_PHONGSHADERUNIFORMS

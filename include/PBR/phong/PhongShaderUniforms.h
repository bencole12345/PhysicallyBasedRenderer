#ifndef PHYSICALLYBASEDRENDERER_PHONGSHADERUNIFORMS
#define PHYSICALLYBASEDRENDERER_PHONGSHADERUNIFORMS

#include <optional>
#include <vector>

#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "core/Material.h"
#include "core/ShaderProgram.h"
#include "core/Texture.h"

namespace PBR::phong {

struct LightingInfo {
    glm::vec3 ambientLight;
    std::vector<glm::vec3> pointLightPositions;
    std::vector<glm::vec3> pointLightColours;
};

struct PhongShaderUniforms {
    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
    glm::vec3 cameraPosition;
    Material material;
    LightingInfo lightingInfo;
    std::optional<unsigned int> textureId;
    std::optional<glm::vec3> colour;
};

void writeUniformsToShaderProgram(const PhongShaderUniforms& uniforms, ShaderProgram& shaderProgram);

} // namespace PBR::phong

#endif //PHYSICALLYBASEDRENDERER_PHONGSHADERUNIFORMS

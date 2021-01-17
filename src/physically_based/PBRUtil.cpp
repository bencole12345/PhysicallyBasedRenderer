#include "physically_based/PBRUtil.h"

#include <filesystem>

#include <glm/ext/scalar_constants.hpp>
#include <glm/trigonometric.hpp>

namespace fs = std::filesystem;

namespace PBR::physically_based {

glm::vec3 PBRUtil::uvToCartesian(glm::vec2 uv)
{
    float u = uv.x;
    float v = uv.y;

    auto phi = 2.0f * glm::pi<float>() * (-0.5f + u);
    auto theta = glm::pi<float>() * v;

    auto x = glm::sin(theta) * glm::sin(phi);
    auto y = glm::cos(theta);
    auto z = -glm::sin(theta) * glm::cos(phi);

    return glm::vec3(x, y, z);
}

const std::filesystem::path& PBRUtil::pbrShadersDir()
{
    static auto path = fs::current_path() / "src" / "physically_based" / "shaders";
    return path;
}

} // namespace PBR::physically_based

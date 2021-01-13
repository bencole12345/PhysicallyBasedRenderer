#include "physically_based/EnvironmentMap.h"

#include <filesystem>
#include <optional>

#include "core/DirectedLightSource.h"
#include "core/Texture.h"

namespace fs = std::filesystem;

namespace PBR::physically_based {

EnvironmentMap::EnvironmentMap(const fs::path& skyboxTexturePath,
                               const fs::path& diffuseIrradianceMapTexturePath,
                               const fs::path& specularIrradianceMapTexturePath,
                               std::optional<DirectedLightSource> sun)
        :skyboxTexture(new Texture(skyboxTexturePath)),
         diffuseIrradianceMap(new Texture(diffuseIrradianceMapTexturePath, true)),
         specularIrradianceMap(new Texture(specularIrradianceMapTexturePath, true)),
         sun(sun)
{
}

std::shared_ptr<Texture> EnvironmentMap::getSkyboxTexture() const
{
    return skyboxTexture;
}

std::shared_ptr<Texture> EnvironmentMap::getDiffuseIrradianceMapTexture() const
{
    return diffuseIrradianceMap;
}

std::shared_ptr<Texture> EnvironmentMap::getSpecularIrradianceMapTexture() const
{
    return specularIrradianceMap;
}

const std::optional<DirectedLightSource>& EnvironmentMap::getSun() const
{
    return sun;
}

} // namespace PBR::physically_based

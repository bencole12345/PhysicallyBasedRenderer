#ifndef PHYSICALLYBASEDRENDERER_ENVIRONMENTMAP
#define PHYSICALLYBASEDRENDERER_ENVIRONMENTMAP

#include <filesystem>
#include <memory>
#include <optional>

#include "core/DirectedLightSource.h"
#include "core/Texture.h"

namespace PBR::physically_based {

/**
 * Wraps the lighting information required to describe an HDR scene for
 * image-based lighting.
 */
class EnvironmentMap {
private:
    /**
     * The underlying HDR texture.
     */
    std::shared_ptr<Texture> backgroundTexture;

    /**
     * The irradiance map for diffuse lighting.
     */
    std::shared_ptr<Texture> diffuseIrradianceMap;

    /**
     * The irradiance map for specular lighting.
     */
    std::shared_ptr<Texture> specularIrradianceMap;

    /**
     * The sun's light source, if it exists.
     */
    std::optional<DirectedLightSource> sun;

public:
    explicit EnvironmentMap(const std::filesystem::path& texturePath,
                            std::optional<DirectedLightSource> sun = std::nullopt);

    std::shared_ptr<Texture> getBackgroundTexture() const;

    std::shared_ptr<Texture> getDiffuseIrradianceMapTexture() const;

    std::shared_ptr<Texture> getSpecularIrradianceMapTexture() const;

    const std::optional<DirectedLightSource>& getSun() const;
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_ENVIRONMENTMAP

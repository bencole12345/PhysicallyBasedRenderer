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
    std::shared_ptr<Texture> radianceMap;

    /**
     * The irradiance map for diffuse lighting.
     *
     * This is the integral of Li(p, wi) * (n dot wi) precomputed for each hemisphere.
     */
    std::shared_ptr<Texture> irradianceMap;

    /**
     * The sun's light source, if it exists.
     */
    std::optional<DirectedLightSource> sun;

public:
    explicit EnvironmentMap(const std::filesystem::path& texturePath,
                            std::optional<DirectedLightSource> sun = std::nullopt);

    std::shared_ptr<Texture> getRadianceMap() const;

    std::shared_ptr<Texture> getIrradianceMap() const;

    const std::optional<DirectedLightSource>& getSun() const;
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_ENVIRONMENTMAP

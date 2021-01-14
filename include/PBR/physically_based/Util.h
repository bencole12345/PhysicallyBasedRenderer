#ifndef PHYSICALLYBASEDRENDERER_UTIL
#define PHYSICALLYBASEDRENDERER_UTIL

#include <filesystem>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace PBR::physically_based {

/**
 * Miscellaneous helper functions used in the physically_based namespace.
 */
struct Util {
public:
    /**
     * Convert from UV (polar) coordinates in a spherically projected texture into
     * a direction vector in Cartesian coordinates.
     *
     * This is useful for decoding the sun's UV position into a direction vector
     * for use in the fragment shader.
     *
     * @param uv The polar coordinates to map from
     * @return The corresponding normalised vector in Cartesian coordinates pointing
     *         in the same direction
     */
    static glm::vec3 uvToCartesian(glm::vec2 uv);

    /**
     * @return The path where the physically based shaders are located
     */
    static const std::filesystem::path& getPhysicallyBasedShadersDirectory();
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_UTIL

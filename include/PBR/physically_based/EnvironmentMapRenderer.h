#ifndef PHYSICALLYBASEDRENDERER_ENVIRONMENTMAPRENDERER
#define PHYSICALLYBASEDRENDERER_ENVIRONMENTMAPRENDERER

#include <memory>

#include "core/Camera.h"
#include "core/ShaderProgram.h"
#include "physically_based/EnvironmentMap.h"

namespace PBR::physically_based {

/**
 * Contains helpers for working with `EnvironmentMap`s.
 */
class EnvironmentMapRenderer {
private:
    /**
     * The program we use to render the skybox.
     */
    ShaderProgram skyboxRenderingShader;

    /**
     * The vertex array object for the skybox's cube vertices.
     */
    unsigned int vaoId;

    /**
     * The vertex buffer object for the skybox's cube vertices.
     */
    unsigned int vboId;

public:
    EnvironmentMapRenderer();
    ~EnvironmentMapRenderer();

    EnvironmentMapRenderer(const EnvironmentMapRenderer& other) = delete;
    EnvironmentMapRenderer& operator=(const EnvironmentMapRenderer& other) = delete;

    void renderSkybox(const std::shared_ptr<EnvironmentMap>& environmentMap, const Camera& camera);
};

} // namespace PBR::physically_based

#endif //PHYSICALLYBASEDRENDERER_ENVIRONMENTMAPRENDERER
